import bpy
import os
import string
import re

from .operatorHelpers import *

class setAcid(bpy.types.Operator):
    bl_idname = "wm.set_acid"
    bl_label = "Mark the selection as a volume of acid."

    def execute(self, context):
        mat = getMaterial('~/.glportal/data/textures/fluid/acid00.png', (0.2, 1.0, 0.2))
        bpy.types.Object.glpType = bpy.props.StringProperty()
        object = bpy.context.active_object
        if object:
            object.glpTypes = "volume"
            object.glpVolumeTypes = "acid"
            me = object.data
            if (len(me.materials) == 0):
                me.materials.append(mat)
            else:
                me.materials[0] = mat
        return {'FINISHED'}
