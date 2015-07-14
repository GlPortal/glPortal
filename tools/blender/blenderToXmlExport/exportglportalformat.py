import bpy
from bpy_extras.io_utils import ExportHelper
import os
import xml.etree.cElementTree as tree
import xml.dom.minidom as minidom
import mathutils
import math
import string
from mathutils import Vector
import re

def storePosition(element, object):
    element.set("x", str(object.location[0]))
    element.set("y", str(object.location[2]))
    element.set("z", str(-object.location[1]))

def storeRotation(element, object):
    element.set("x", str(abs(math.degrees(object.rotation_euler[0]))))
    element.set("y", str(abs(math.degrees(object.rotation_euler[2]))))
    element.set("z", str(abs(math.degrees(-object.rotation_euler[1]))))

def storeScale(element, object):
    element.set("x", str(object.dimensions[0]))
    element.set("y", str(object.dimensions[2]))
    element.set("z", str(object.dimensions[1]))

def writeLampToTree(object, targetTree):
    lamp = object.data
    
    colorArray = lamp.color
    lightDistance = lamp.distance
    lightEnergy = lamp.energy
                
    lightElement = tree.SubElement(targetTree, "light")
    storePosition(lightElement, object);
                
    lightElement.set("r", str(colorArray[0]))
    lightElement.set("g", str(colorArray[1]))
    lightElement.set("b", str(colorArray[2]))
                
    lightElement.set("distance", str(lightDistance))
    lightElement.set("energy", str(lightEnergy))

class ExportGlPortalFormat(bpy.types.Operator, ExportHelper):
    bl_idname = "export_glportal_xml.xml"
    bl_label = "GlPortal XML Format"
    bl_options = {'PRESET'}
    filename_ext = ".xml"
    
    def execute(self, context):
        dir = os.path.dirname(self.filepath)
        objects = context.scene.objects
        root = tree.Element("map")
        
        # Materials
        materialElement = tree.SubElement(root, "materials")
        material1 = tree.SubElement(materialElement, "mat")
        material2 = tree.SubElement(materialElement, "mat")
        
        material1.set("mid", "1")
        material1.set("name", "concrete/wall00")
        material2.set("mid", "2")
        material2.set("name", "metal/tiles00x3")

        # Exporting
        for object in objects:
            object.select = False
        for object in objects:
            if object.glpTypes:
                type = object.glpTypes
            else:
                type = "None"                 
            if object.type == "LAMP":
                writeLampToTree(object, root)
            if object.type == "CAMERA":
                boxElement = tree.SubElement(root, "spawn")
                
                positionElement = tree.SubElement(boxElement, "position")
                storePosition(positionElement, object);
                
                rotationElement = tree.SubElement(boxElement, "rotation")
                storeRotation(rotationElement, object);
            if object.type == "MESH":
                if type == "trigger":
                    boxElement = tree.SubElement(root, "trigger")
                    if object.glpTriggerTypes:
                        boxElement.set("type", object.glpTriggerTypes)
                elif type == "wall":
                    print(object.glpWallTypes)
                    if object.glpWallTypes == "portable":
                        boxElement = tree.SubElement(root, "wall")
                        boxElement.set("mid", "1")
                    else:
                        boxElement = tree.SubElement(root, "wall")
                        boxElement.set("mid", "2")
                elif type == "volume":
                    print(object.glpVolumeTypes)
                    if object.glpVolumeTypes == "acid":
                        boxElement = tree.SubElement(root, "acid")
                else:
                    boxElement = tree.SubElement(root, "door")
                
                object.select = True

                positionElement = tree.SubElement(boxElement, "position")
                storePosition(positionElement, object);
                
                rotationElement = tree.SubElement(boxElement, "rotation")
                storeRotation(rotationElement, object);
                
                scaleElement = tree.SubElement(boxElement, "scale")
                storeScale(scaleElement, object);

                object.select = False
        
        xml = minidom.parseString(tree.tostring(root))

        file = open(self.filepath, "w")
        fix = re.compile(r'((?<=>)(\n[\t]*)(?=[^<\t]))|(?<=[^>\t])(\n[\t]*)(?=<)')
        fixed_output = re.sub(fix, '', xml.toprettyxml())
        file.write(fixed_output)
        file.close()

        return {'FINISHED'}
