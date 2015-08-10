import bpy
import os
import string
import re
from .paths import *
from .operatorHelpers import *

class addDoor(bpy.types.Operator):
    bl_idname = "wm.add_door"
    bl_label = "Add a door."
    bl_options = {"UNDO"}
    
    def execute(self, context):
        realpath = os.path.expanduser(Paths.GLPORTAL_DATA_DIR + "meshes/Door.obj")
        bpy.ops.import_scene.obj(filepath=realpath)
        bpy.ops.transform.translate(value=(bpy.context.scene.cursor_location))
        bpy.types.Object.glpType = bpy.props.StringProperty()
        
        # make sure to get all imported objects
        obj_objects = bpy.context.selected_objects[:]
        
        # iterate through all objects to find new
        for object in obj_objects:
            if object.glpTypes == "none":
                object.glpTypes = "door"
        
        return {'FINISHED'}

class addLamp(bpy.types.Operator):
    bl_idname = "wm.add_lamp"
    bl_label = "Add a lamp."
    bl_options = {"UNDO"}
    
    def execute(self, context):
        realpath = os.path.expanduser(Paths.GLPORTAL_DATA_DIR + "meshes/Lamp.obj")
        bpy.ops.import_scene.obj(filepath=realpath)
        bpy.ops.transform.translate(value=(bpy.context.scene.cursor_location))
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object.glpTypes = "lamp"
        return {'FINISHED'}

class addButton(bpy.types.Operator):
    bl_idname = "wm.add_button"
    bl_label = "Add a button."
    bl_options = {"UNDO"}
    
    def execute(self, context):
        realpath = os.path.expanduser(Paths.GLPORTAL_DATA_DIR + "meshes/Button.obj")
        bpy.ops.import_scene.obj(filepath=realpath)
        bpy.ops.transform.translate(value=(bpy.context.scene.cursor_location))
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object.glpTypes = "button"
        return {'FINISHED'}

class setPortable(bpy.types.Operator):
    bl_idname = "wm.set_portable"
    bl_label = "Mark the selection as portable."
    bl_options = {"UNDO"}
    
    def execute(self, context):
        mat = getMaterial(Paths.GLPORTAL_DATA_DIR + 'textures/concrete/wall00.png', (1, 1, 1))
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
    bl_options = {"UNDO"}
    
    def execute(self, context):
        mat = getMaterial(Paths.GLPORTAL_DATA_DIR + 'textures/metal/tiles00x3.jpg', (0.2, 0.2, 0.2))
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

class addWall(bpy.types.Operator):
    bl_idname = "wm.add_wall"
    bl_label = "Add a wall."
    bl_options = {"UNDO"}
    
    def execute(self, context):
        bpy.ops.mesh.primitive_cube_add()
        setWall.execute(self, context)
        return {'FINISHED'}

class addPortable(bpy.types.Operator):
    bl_idname = "wm.add_portable"
    bl_label = "Add a portable wall."
    bl_options = {"UNDO"}
    
    def execute(self, context):
        bpy.ops.mesh.primitive_cube_add()
        setPortable.execute(self, context)
        return {'FINISHED'}
