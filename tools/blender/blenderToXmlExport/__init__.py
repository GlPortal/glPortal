#!BPY
bl_info = {
    "name":         "GlPortal XML Format",
    "author":       "Henry Hirsch",
    "blender":      (2, 6, 3),
    "version":      (0, 0, 1),
    "location":     "File > Import-Export",
    "description":  "GlPortal XML Format",
    "category":     "Import-Export"
}

import bpy
from bpy_extras.io_utils import ExportHelper
import xml.etree.cElementTree as tree
import xml.dom.minidom as minidom
import os
import mathutils

class ExportMyFormat(bpy.types.Operator, ExportHelper):
    bl_idname = "export_glportal_xml.xml"
    bl_label = "GlPortal XML Format"
    bl_options = {'PRESET'}
    filename_ext = ".xml"

    def execute(self, context):
       dir = os.path.dirname(self.filepath)
       objects = context.scene.objects
       root = tree.Element("map")
       textureElement = tree.SubElement(root, "texture")
       for object in objects:
           object.select = False
       for object in objects:
           if object.name == "Lamp":
               lightElement = tree.SubElement(root, "light")
               lightVectorElement = tree.SubElement(lightElement, "vector")               
               lightVectorElement.set("x", str(object.location[0]))
               lightVectorElement.set("y", str(object.location[1]))
               lightVectorElement.set("z", str(object.location[2]))
           else:    
               object.select = True
               boxElement = tree.SubElement(textureElement, "box")
               boundingBox = object.bound_box
               vectorElement = tree.SubElement(boxElement, "vector")
               vectorElement.set("x", str(boundingBox[0][0]))
               vectorElement.set("y", str(boundingBox[0][1]))
               vectorElement.set("z", str(boundingBox[0][2]))
               endVectorElement = tree.SubElement(boxElement, "vector")
               endVectorElement.set("x", str(boundingBox[1][0]))
               endVectorElement.set("y", str(boundingBox[1][1]))
               endVectorElement.set("z", str(boundingBox[1][2]))
               object.select = False
               
       xml = minidom.parseString(tree.tostring(root))

       file = open(self.filepath, "w")
       file.write(xml.toprettyxml())
       file.close()

       return {'FINISHED'}

    
def menu_func(self, context):
    self.layout.operator(ExportMyFormat.bl_idname, text="GlPortal Map Format(.xml)")


def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)


def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()
