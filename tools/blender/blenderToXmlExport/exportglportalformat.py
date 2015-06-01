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

class ExportGlPortalFormat(bpy.types.Operator, ExportHelper):
    bl_idname = "export_glportal_xml.xml"
    bl_label = "GlPortal XML Format"
    bl_options = {'PRESET'}
    filename_ext = ".xml"
    
    def execute(self, context):
        dir = os.path.dirname(self.filepath)
        objects = context.scene.objects
        root = tree.Element("map")
        textureElement = tree.SubElement(root, "texture")
        textureElement.set("source", "tiles.png")

        textureWallElement = tree.SubElement(root, "texture")
        textureWallElement.set("source", "wall.png")

        for object in objects:
            object.select = False
        for object in objects:
            if object.glpTypes:
                type = object.glpTypes
            else:
                type = "None"

            if object.type == "LAMP":
                lamp = object.data
                
                colorArray = lamp.color
                lightDistance = lamp.distance
                lightEnergy = lamp.energy
                
                lightElement = tree.SubElement(root, "light")
                storePosition(lightElement, object);
                
                lightElement.set("r", str(colorArray[0]))
                lightElement.set("g", str(colorArray[1]))
                lightElement.set("b", str(colorArray[2]))
                
                lightElement.set("distance", str(lightDistance))
                lightElement.set("energy", str(lightEnergy))
            if object.type == "CAMERA":
                boxElement = tree.SubElement(root, "spawn")
                
                positionElement = tree.SubElement(boxElement, "position")
                storePosition(positionElement, object);
                
                rotationElement = tree.SubElement(boxElement, "rotation")
                storeRotation(rotationElement, object);
            if object.type == "MESH":
                if type == "trigger":
                    if object.glpTriggerTypes == "win":
                        boxElement = tree.SubElement(root, "end")
                    else:
                        boxElement = tree.SubElement(root, "trigger")
                elif type == "wall":
                    print(object.glpWallTypes)
                    if object.glpWallTypes == "portable":
                        boxElement = tree.SubElement(textureWallElement, "wall")
                    else:
                        boxElement = tree.SubElement(textureElement, "wall")
                elif type == "volume":
                    print(object.glpVolumeTypes)
                    if object.glpVolumeTypes == "acid":
                        boxElement = tree.SubElement(root, "acid")
                else:
                    boxElement = tree.SubElement(textureElement, "wall")
                
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
