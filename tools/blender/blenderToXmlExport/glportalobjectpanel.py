import bpy
from bpy.props import *
from .types import *

class GlPortalObjectPanel(bpy.types.Panel):
    """GlPortal panel in the toolbar"""
    bl_label = "GlPortal"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "object"

    def draw(self, context):
        object = context.active_object
        layout = self.layout
        row = layout.row()

        layout.prop(object, "glpTypes")
        if object.glpTypes == "trigger":
            layout.prop(object, "glpTriggerTypes")
        if object.glpTypes == "wall":
            layout.prop(object, "glpWallTypes")
            
