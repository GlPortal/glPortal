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
               hasType = True;
           else:
               type = "None"
               hasType = False;
               
           if "glpTriggerType" in object:
               triggerType = object["glpTriggerType"]
               hasTriggerType = True;
           else:
               triggerType = "None"
               hasTriggerType = False;
               
           if "." in object.name:
               mapObjectType = object.name.split(".")[0]
           else:
               mapObjectType = object.name
               
           if mapObjectType == "Lamp":
               lightElement = tree.SubElement(root, "light")
               colorArray = object.data.color
               lightElement.set("r", str(colorArray[0]))
               lightElement.set("g", str(colorArray[1]))
               lightElement.set("b", str(colorArray[2]))
               
               lightElement.set("x", str(object.location[0]))
               lightElement.set("y", str(object.location[2]))
               lightElement.set("z", str(-object.location[1]))
           elif mapObjectType == "Cube" or mapObjectType == "Camera":
               if mapObjectType == "Camera":
                   boxElement = tree.SubElement(root, "spawn")
               elif type == "trigger":                       
                   boxElement = tree.SubElement(root, "trigger")
                   if "glpTriggerType" in object:
                       boxElement.set("type", triggerType)
               elif type == "door":
                   boxElement = tree.SubElement(root, "end")
               elif type == "portable":
                   boxElement = tree.SubElement(textureWallElement, "wall")
               else:
                   boxElement = tree.SubElement(textureElement, "wall")
                   
               object.select = True

               positionElement = tree.SubElement(boxElement, "position")
               positionElement.set("x", str(object.location[0]))
               positionElement.set("y", str(object.location[2]))
               positionElement.set("z", str(-object.location[1]))
               if type != "door" and mapObjectType != "Camera":
                   scaleElement = tree.SubElement(boxElement, "scale")
                   scaleElement.set("x", str(object.dimensions[0]))
                   scaleElement.set("y", str(object.dimensions[2]))
                   scaleElement.set("z", str(object.dimensions[1]))
               if type == "door" or mapObjectType == "Camera":
                   rotationElement = tree.SubElement(boxElement, "rotation")
                   rotationElement.set("x", str(abs(math.degrees(object.rotation_euler[0]))))
                   rotationElement.set("y", str(abs(math.degrees(object.rotation_euler[2]))))
                   rotationElement.set("z", str(abs(math.degrees(-object.rotation_euler[1]))))
                   
               object.select = False
               
       xml = minidom.parseString(tree.tostring(root))

       file = open(self.filepath, "w")
       fix = re.compile(r'((?<=>)(\n[\t]*)(?=[^<\t]))|(?<=[^>\t])(\n[\t]*)(?=<)')
       fixed_output = re.sub(fix, '', xml.toprettyxml())
       file.write(fixed_output)
       file.close()

       return {'FINISHED'}
