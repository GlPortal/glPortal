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
import string
from mathutils import Vector
import re

class CustomPanel(bpy.types.Panel):
    """GlPortal panel in the toolbar"""
    bl_label = "GlPortal"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'

    def draw(self, context):
        layout = self.layout
#        self.layout.prop(bpy.context.active_object, '["gl_type"]')
        row = layout.row()
        row.label(text="Cube properties:")

        split = layout.split()
        col = split.column(align=True)

#        col.operator("pipo", text="Plane", icon='MESH_PLANE')
        col.operator("wm.clear_selection", text="Clear Type", icon='MESH_CUBE')
        col.operator("wm.selection_to_win", text="Set Win Area", icon='MESH_CUBE')
        col.operator("wm.selection_to_death", text="Set Death Area", icon='MESH_CUBE')
        col.operator("wm.selection_to_radiation", text="Set Radiation Area", icon='MESH_CUBE')

class clearSelection(bpy.types.Operator):
    bl_idname = "wm.clear_selection"
    bl_label = "Mark the selection as neutral area."

    def execute(self, context):
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object["glpType"] = "None" 
        return {'FINISHED'}
        
class selectionToWin(bpy.types.Operator):
    bl_idname = "wm.selection_to_win"
    bl_label = "Mark the selection as winning area."

    def execute(self, context):
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object["glpType"] = "win" 
        return {'FINISHED'}

class selectionToDeath(bpy.types.Operator):
    bl_idname = "wm.selection_to_death"
    bl_label = "Mark the selection as death area."

    def execute(self, context):
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object["glpType"] = "death" 
        return {'FINISHED'}

    
class selectionToRadiation(bpy.types.Operator):
    bl_idname = "wm.selection_to_radiation"
    bl_label = "Mark the selection as radiation area."

    def execute(self, context):
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object["glpType"] = "radiation" 
        return {'FINISHED'}    
    
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
       textureElement.set("source", "tiles.png")
       for object in objects:
           object.select = False
       for object in objects:
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
               matrix = object.matrix_world
               vector = Vector((object.location[0],-object.location[2], object.location[1]))
               globalVector = matrix * vector
               lightElement.set("x", str(globalVector.x))
               lightElement.set("y", str(globalVector.z))
               lightElement.set("z", str(globalVector.y))
           elif mapObjectType == "Camera":
               matrix = object.matrix_world
               vector = Vector((object.location[0], -object.location[2], object.location[1]))
               globalVector = matrix * vector
               spawnElement = tree.SubElement(root, "spawn")
               spawnElement.set("x", str(globalVector.x))
               spawnElement.set("y", str(globalVector.z))
               spawnElement.set("z", str(globalVector.y))               
           elif mapObjectType == "Cube":
               matrix = object.matrix_world
               boundingBox = object.bound_box

               boundingBoxBeginVector = Vector((boundingBox[0][0], -boundingBox[0][2], boundingBox[0][1]))
               boundingBoxEndVector  = Vector((boundingBox[6][0], -boundingBox[6][2], boundingBox[6][1]))
               transBoundingBoxBeginVector = matrix * boundingBoxBeginVector
               transBoundingBoxEndVector  = matrix * boundingBoxEndVector
               object.select = True
               if hasattr(object, 'glpType'):
                   if object.glpType != "None":
                       boxElement = tree.SubElement(root, "trigger")
                       boxElement.set("type", object.glpType)
               else:
                   boxElement = tree.SubElement(textureElement, "wall")
                   
               boundingBox = object.bound_box
             
               positionElement = tree.SubElement(boxElement, "position")
               positionElement.set("x", str((transBoundingBoxEndVector.x + transBoundingBoxBeginVector.x)/2))
               positionElement.set("y", str((transBoundingBoxEndVector.z + transBoundingBoxBeginVector.z)/2))
               positionElement.set("z", str((transBoundingBoxEndVector.y + transBoundingBoxBeginVector.y)/2))
               scaleElement = tree.SubElement(boxElement, "scale")
               scaleElement.set("x", str(abs(transBoundingBoxEndVector.x - transBoundingBoxBeginVector.x)))
               scaleElement.set("y", str(abs(transBoundingBoxEndVector.z - transBoundingBoxBeginVector.z)))
               scaleElement.set("z", str(abs(transBoundingBoxEndVector.y - transBoundingBoxBeginVector.y)))
               object.select = False
               
       xml = minidom.parseString(tree.tostring(root))

       file = open(self.filepath, "w")
       fix = re.compile(r'((?<=>)(\n[\t]*)(?=[^<\t]))|(?<=[^>\t])(\n[\t]*)(?=<)')
       fixed_output = re.sub(fix, '', xml.toprettyxml())
       file.write(fixed_output)
       file.close()

       return {'FINISHED'}

    
def menu_func(self, context):
    self.layout.operator(ExportMyFormat.bl_idname, text="GlPortal Map Format(.xml)")


def register():
#    bpy.types.Object.gl_type = bpy.props.StringProperty()
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)
    bpy.utils.register_class(CustomPanel)
    bpy.utils.register_class(SimpleOperator)


def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)
    bpy.utils.unregister_class(CustomPanel)
    bpy.utils.unregister_class(SimpleOperator)

if __name__ == "__main__":
    register()
