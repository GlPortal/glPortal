import bpy
import os
import string
import re

def setTrigger(trigger):
    bpy.types.Object.glpType = bpy.props.StringProperty()
    object = bpy.context.active_object
    if object:
        object["glpType"] = "trigger"
        object["glpTriggerType"] = trigger     

class clearSelection(bpy.types.Operator):
    bl_idname = "wm.clear_selection"
    bl_label = "Mark the selection as neutral area."

    def execute(self, context):
        bpy.types.Object.glpType = bpy.props.StringProperty()
        bpy.types.Object.glpTriggerType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object["glpType"] = "None"
            object["glpTriggerType"] = "None"             
        return {'FINISHED'}
        
class selectionToWin(bpy.types.Operator):
    bl_idname = "wm.selection_to_win"
    bl_label = "Mark the selection as winning area."

    def execute(self, context):
        setTrigger("win")
        return {'FINISHED'}

class selectionToDeath(bpy.types.Operator):
    bl_idname = "wm.selection_to_death"
    bl_label = "Mark the selection as death area."

    def execute(self, context):
        setTrigger("death")
        return {'FINISHED'}

    
class selectionToRadiation(bpy.types.Operator):
    bl_idname = "wm.selection_to_radiation"
    bl_label = "Mark the selection as radiation area."

    def execute(self, context):
        setTrigger("radiation")
        return {'FINISHED'}    

class selectionToDoor(bpy.types.Operator):
    bl_idname = "wm.selection_to_door"
    bl_label = "Mark the selection as door."

    def execute(self, context):
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object["glpTriggerType"] = "None"
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
            object["glpTriggerType"] = "None"
            object["glpType"] = "portable"
            me = object.data
            me.materials.append(mat)
        return {'FINISHED'}    
