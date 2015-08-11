import bpy
from bpy.props import *
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
    element.set("x", str(round(object.location[0], 5)))
    element.set("y", str(round(object.location[2], 5)))
    element.set("z", str(-round(object.location[1], 5)))

def storeRotation(element, object):
    element.set("x", str(round(abs(math.degrees(object.rotation_euler[0])), 5)))
    element.set("y", str(round(abs(math.degrees(object.rotation_euler[2])), 5)))
    element.set("z", str(round(abs(math.degrees(-object.rotation_euler[1])), 5)))

def storeScale(element, object):
    element.set("x", str(round(object.dimensions[0], 5)))
    element.set("y", str(round(object.dimensions[2], 5)))
    element.set("z", str(round(object.dimensions[1], 5)))

def writeLampToTree(object, targetTree):
    lamp = object.data
    
    colorArray = lamp.color
    lightDistance = lamp.distance
    lightEnergy = lamp.energy
    
    lightElement = tree.SubElement(targetTree, "light")
    storePosition(lightElement, object);
    
    lightElement.set("r", str(round(colorArray[0], 5)))
    lightElement.set("g", str(round(colorArray[1], 5)))
    lightElement.set("b", str(round(colorArray[2], 5)))
    
    lightElement.set("distance", str(round(lightDistance, 5)))
    lightElement.set("energy", str(round(lightEnergy, 5)))
    
    if lamp.use_specular:
        lightElement.set("specular", "1")

class ExportGlPortalFormat(bpy.types.Operator, ExportHelper):
    bl_idname = "export_glportal_xml.xml"
    bl_label = "Export GlPortal XML"
    bl_description = "Export to GlPortal XML file (.xml)"
    bl_options = {'PRESET'}
    filename_ext = ".xml"
    filter_glob = StringProperty(default="*.xml", options={'HIDDEN'})
    
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
            elif object.type == "CAMERA":
                boxElement = tree.SubElement(root, "spawn")
                
                positionElement = tree.SubElement(boxElement, "position")
                storePosition(positionElement, object);
                
                rotationElement = tree.SubElement(boxElement, "rotation")
                rotationElement.set("x", str(round(abs(math.degrees(object.rotation_euler[2])), 5)))
                rotationElement.set("y", str(round(abs(math.degrees(object.rotation_euler[0]) - 90), 5)))
                rotationElement.set("z", str(round(abs(math.degrees(-object.rotation_euler[1]) - 180), 5)))
            elif object.type == "MESH" and type == "door":
                """tempotary add <end> instead of <door>"""
                boxElement = tree.SubElement(root, "end")
                
                positionElement = tree.SubElement(boxElement, "position")
                storePosition(positionElement, object);
                
                rotationElement = tree.SubElement(boxElement, "rotation")
                rotationElement.set("x", str(round(abs(math.degrees(object.rotation_euler[0]) - 90), 5)))
                rotationElement.set("y", str(round(abs(math.degrees(object.rotation_euler[2])), 5)))
                rotationElement.set("z", str(round(abs(math.degrees(-object.rotation_euler[1])), 5)))
            elif object.type == "MESH":
                if type == "trigger":
                    boxElement = tree.SubElement(root, "trigger")
                    if object.glpTriggerTypes:
                        boxElement.set("type", object.glpTriggerTypes)
                elif type == "wall":
                    #print(object.glpWallTypes)
                    boxElement = tree.SubElement(root, "wall")
                    if object.glpWallTypes == "portalable":
                        boxElement.set("mid", "1")
                    else:
                        boxElement.set("mid", "2")
                elif type == "volume":
                    #print(object.glpVolumeTypes)
                    if object.glpVolumeTypes == "acid":
                        boxElement = tree.SubElement(root, "acid")
                 # disabled, will be enabled in the future
#                else:
#                    boxElement = tree.SubElement(root, "door")
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
