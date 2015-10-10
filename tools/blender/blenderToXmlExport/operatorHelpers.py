import bpy
import os

def clearGlpProperties():
    object = bpy.context.active_object
    if object:
        object.glpTypes = "none"
        object.glpVolumeTypes = "none"
        object.glpTriggerTypes = "none"
        object.glpWallTypes = "none"        

def getMaterial(texturePath, diffuse):
    realpath = os.path.expanduser(texturePath)
    try:
        portableWallImage = bpy.data.images.load(realpath)
    except:
        raise NameError("Cannot load image %s" % realpath)
    
    portableWallTexture = bpy.data.textures.new('ColorTex', type = 'IMAGE')
    portableWallTexture.image = portableWallImage
    
    mat = bpy.data.materials.new('TexMat')
    mat.diffuse_color = diffuse

    # Add texture slot for color texture
    mtex = mat.texture_slots.add()
    mtex.texture = portableWallTexture
    mtex.texture_coords = 'UV'
    mtex.use_map_color_diffuse = True 
    mtex.use_map_color_emission = True 
    mtex.emission_color_factor = 0.5
    mtex.use_map_density = True 
    mtex.mapping = 'FLAT'
    return mat
