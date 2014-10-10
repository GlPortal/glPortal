import bpy

class GlPortalPanel(bpy.types.Panel):
    """GlPortal panel in the toolbar"""
    bl_label = "GlPortal"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'

    def draw(self, context):
        layout = self.layout
        row = layout.row()
        row.label(text="Cube properties:")

        split = layout.split()
        col = split.column(align=True)

        col.operator("wm.clear_selection", text="Clear Type", icon='MESH_CUBE')
        col.operator("wm.selection_to_win", text="Set Win Area", icon='MESH_CUBE')
        col.operator("wm.selection_to_death", text="Set Death Area", icon='MESH_CUBE')
        col.operator("wm.selection_to_radiation", text="Set Radiation Area", icon='MESH_CUBE')
        col.operator("wm.selection_to_door", text="Set Door", icon='MESH_CUBE')
        col.operator("wm.selection_to_portable", text="Set Wall Portable", icon='MESH_CUBE')
