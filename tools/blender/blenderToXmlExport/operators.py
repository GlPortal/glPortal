import bpy
import os
import string
import re

from .operatorHelpers import *
        
class addDoor(bpy.types.Operator):
    bl_idname = "wm.add_door"
    bl_label = "Mark the selection as door."

    def execute(self, context):
        realpath = os.path.expanduser("~/.glportal/data/meshes/Door.obj")
        bpy.ops.import_scene.obj(filepath=realpath)
        bpy.ops.transform.translate(value=(bpy.context.scene.cursor_location))
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object.glpTypes = "door" 
        return {'FINISHED'}

class setPortable(bpy.types.Operator):
    bl_idname = "wm.set_portable"
    bl_label = "Mark the selection as portable."
    
    def execute(self, context):
        mat = getMaterial('~/.glportal/data/textures/wall.png', (1, 1, 1))
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object.glpTypes = "wall"
            object.glpWallTypes = "portable"
            me = object.data
            if (len(me.materials) == 0):
                me.materials.append(mat)
            else:
                me.materials[0] = mat
        return {'FINISHED'}    

class setWall(bpy.types.Operator):
    bl_idname = "wm.set_wall"
    bl_label = "Mark the selection as portable."

    def execute(self, context):
        mat = getMaterial('~/.glportal/data/textures/tiles.png', (0.2, 0.2, 0.2))
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object.glpTypes = "wall"
            object.glpWallTypes = "default"
            me = object.data
            if (len(me.materials) == 0):
                me.materials.append(mat)
            else:
                me.materials[0] = mat
        return {'FINISHED'}
