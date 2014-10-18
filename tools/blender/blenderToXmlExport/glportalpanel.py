import bpy

class GlPortalPanel(bpy.types.Panel):
    """GlPortal panel in the toolbar"""
    bl_label = "GlPortal Quick Bar"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'

    def draw(self, context):
        layout = self.layout
        row = layout.row()
        split = layout.split()
        col = split.column(align=True)
        col.operator("wm.add_door", text="Add Door", icon='MESH_CUBE')
        col.operator("wm.add_portable", text="Add Portable Wall", icon='MESH_PLANE')
        col.operator("wm.add_wall", text="Add Wall", icon='MESH_PLANE')
