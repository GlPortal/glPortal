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

        layout.prop(object, "types")
        if object.types == "trigger":
            layout.prop(object, "triggerTypes")
            
        split = layout.split()
        col = split.column(align=True)
        col.operator("wm.save_object_type", text="save", icon='FILE_TICK')
        col.operator("wm.clear_selection", text="clear", icon='CANCEL')
