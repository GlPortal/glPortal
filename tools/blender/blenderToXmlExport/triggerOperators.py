import bpy
import os
import string
import re

from .operatorHelpers import *

class setWin(bpy.types.Operator):
    bl_idname = "wm.set_win"
    bl_label = "Mark the selection as win trigger."
    
    def execute(self, context):
        object = bpy.context.active_object
        if object:
            object.glpTypes = "trigger"
            object.glpTriggerTypes = "win"
        return {'FINISHED'}

class addWin(bpy.types.Operator):
    bl_idname = "wm.add_win"
    bl_label = "Mark the selection as win trigger."
    
    def execute(self, context):
        bpy.ops.mesh.primitive_cube_add()
        setWin.execute(self, context)
        return {'FINISHED'}

class setDeath(bpy.types.Operator):
    bl_idname = "wm.set_death"
    bl_label = "Mark the selection as death trigger."
    
    def execute(self, context):
        object = bpy.context.active_object
        if object:
            object.glpTypes = "trigger"
            object.glpTriggerTypes = "death"
        return {'FINISHED'}

class addDeath(bpy.types.Operator):
    bl_idname = "wm.add_death"
    bl_label = "Mark the selection as death trigger."
    
    def execute(self, context):
        bpy.ops.mesh.primitive_cube_add()
        setDeath.execute(self, context)
        return {'FINISHED'}

class addRadiation(bpy.types.Operator):
    bl_idname = "wm.add_radiation"
    bl_label = "Mark the selection as radiation trigger."
    
    def execute(self, context):
        bpy.ops.mesh.primitive_cube_add()
        object = bpy.context.active_object
        if object:
            object.glpTypes = "trigger"
            object.glpTriggerTypes = "radiation"
        return {'FINISHED'}
