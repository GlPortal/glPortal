#!BPY
bl_info = {
    "name":         "GlPortal XML Format",
    "author":       "Henry Hirsch",
    "blender":      (2, 6, 3),
    "version":      (0, 0, 2),
    "location":     "File > Import-Export",
    "description":  "GlPortal XML Format",
    "category":     "Import-Export"
}

import bpy
import xml.etree.cElementTree as tree
import xml.dom.minidom as minidom
import os
import mathutils
import math
import string
from mathutils import Vector
import re
from .glportalpanel import *
from .exportglportalformat import *

class clearSelection(bpy.types.Operator):
    bl_idname = "wm.clear_selection"
    bl_label = "Mark the selection as neutral area."

    def execute(self, context):
        bpy.types.Object.glpType = bpy.props.StringProperty()
        bpy.types.Object.glpTriggerType = bpy.props.StringProperty()
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
            object["glpType"] = "trigger"
            object["glpTriggerType"] = "win" 
        return {'FINISHED'}

class selectionToDeath(bpy.types.Operator):
    bl_idname = "wm.selection_to_death"
    bl_label = "Mark the selection as death area."

    def execute(self, context):
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object["glpType"] = "trigger"
            object["glpTriggerType"] = "death" 
        return {'FINISHED'}

    
class selectionToRadiation(bpy.types.Operator):
    bl_idname = "wm.selection_to_radiation"
    bl_label = "Mark the selection as radiation area."

    def execute(self, context):
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object["glpType"] = "trigger"
            object["glpTriggerType"] = "radiation" 
        return {'FINISHED'}    

class selectionToDoor(bpy.types.Operator):
    bl_idname = "wm.selection_to_door"
    bl_label = "Mark the selection as door."

    def execute(self, context):
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object["glpType"] = "door" 
        return {'FINISHED'}    

class selectionToPortable(bpy.types.Operator):
    bl_idname = "wm.selection_to_portable"
    bl_label = "Mark the selection as portable."

    def execute(self, context):
        realpath = os.path.expanduser('~/.glportal/data/textures/wall.png')
        try:
            portableWallImage = bpy.data.images.load(realpath)
        except:
            raise NameError("Cannot load image %s" % realpath)

        portableWallTexture = bpy.data.textures.new('ColorTex', type = 'IMAGE')
        portableWallTexture.image = portableWallImage
        
        mat = bpy.data.materials.new('TexMat')
 
        # Add texture slot for color texture
        mtex = mat.texture_slots.add()
        mtex.texture = portableWallTexture
        mtex.texture_coords = 'UV'
        mtex.use_map_color_diffuse = True 
        mtex.use_map_color_emission = True 
        mtex.emission_color_factor = 0.5
        mtex.use_map_density = True 
        mtex.mapping = 'FLAT' 
        
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object["glpType"] = "portable"
            me = object.data
            me.materials.append(mat)
        return {'FINISHED'}    
    
    
def menu_func(self, context):
    self.layout.operator(ExportGlPortalFormat.bl_idname, text="GlPortal Map Format(.xml)")


def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)
    bpy.utils.unregister_class(GlPortalPanel)

if __name__ == "__main__":
    register()
