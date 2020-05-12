#  Copyright (C) 2009-2010 Samuel Anjam
#
#  This software is provided 'as-is', without any express or implied
#  warranty.  In no event will the authors be held liable for any damages
#  arising from the use of this software.
#
#  Permission is granted to anyone to use this software for any purpose,
#  including commercial applications, and to alter it and redistribute it
#  freely, subject to the following restrictions:
#
#  1. The origin of this software must not be misrepresented; you must not
#     claim that you wrote the original software. If you use this software
#     in a product, an acknowledgment in the product documentation would be
#     appreciated but is not required.
#  2. Altered source versions must be plainly marked as such, and must not be
#     misrepresented as being the original software.
#  3. This notice may not be removed or altered from any source distribution.

bl_addon_info = {
    "name": "Export BlendELF Format (.pak)",
    "author": "Samuel Anjam (centralnoise)",
    "version": (1,0),
    "blender": (2, 5, 4),
    "api": 31847,
    "location": "File > Export",
    "description": "Export to the BlendELF Format (.pak)",
    "warning": "",
    "wiki_url": "http://blendelf.com",
    "tracker_url": "http://github.com/centralnoise/BlendELF/blob/master/tools/blender25_pak_exporter_fast.py",
    "category": "Import/Export"}

import bpy
import struct
import math
import os

ELF_NAME_LENGTH = 128
ELF_PAK_VERSION = 104

def write_name_to_file(name, f):
	if len(name) > ELF_NAME_LENGTH-1: name = name[0:ELF_NAME_LENGTH-1]
	if len(name) > 0: f.write(struct.pack('<'+str(len(name))+'s', name))
	i = len(name)
	while i < ELF_NAME_LENGTH:
		f.write(struct.pack('<B', 0))
		i += 1

def trim_file_path(file_path):
	nfile_path = file_path
	if len(nfile_path) > 2:
		if nfile_path[0:2] == '//':
			nfile_path = nfile_path[2:]
	if nfile_path.rfind('\\') > -1:
		nfile_path = nfile_path[nfile_path.rfind('\\')+1:]
	if nfile_path.rfind('/') > -1:
		nfile_path = nfile_path[nfile_path.rfind('/')+1:]
	return nfile_path

class Texture:
	def __init__(self):
		self.name = ''
		self.file_path = ''
		self.type = ''
		self.length = 0
		self.data = None
		self.size_bytes = 0

	def load(self, texture):
		self.size_bytes = 0
		tex = texture.texture

		self.name = tex.name

		img = tex.image
		self.file_path = img.filepath
		if len(self.file_path) > 2:
			if self.file_path[0:2] == '//':
				self.file_path = self.file_path[2:]

		try:
			f = open(self.file_path, 'rb')

			self.data = f.read()
			self.length = len(self.data)

			self.file_path = trim_file_path(self.file_path)

			f.close()
		except:
			print('error: could not load texture ' + self.name + ' from \"' + self.file_path + '\"')
			return False

		# magic
		self.size_bytes += struct.calcsize('<i')
		# name
		self.size_bytes += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')
		# image format
		self.size_bytes += struct.calcsize('<B')
		# length
		self.size_bytes += struct.calcsize('<i')
		# data
		self.size_bytes += self.length

		print('Texture \"'+self.name+'\" converted')
		print('  size: '+str(self.size_bytes)+' bytes')

		return True

	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532108))

		# write name
		write_name_to_file(self.name, f)

		# write format
		f.write(struct.pack('<B', 1))	# packed elf image

		# write length of data
		f.write(struct.pack('<i', self.length))

		# write data
		f.write(self.data)

		print('Texture \"'+self.name+'\" saved')

class Material:
	def __init__(self):
		self.name = ''
		self.diffuse = [1.0, 1.0, 1.0, 1.0]
		self.ambient = [0.0, 0.0, 0.0, 1.0]
		self.specular = [0.0, 0.0, 0.0, 1.0]
		self.shininess = 32.0
		self.diffuse_map = ''
		self.normal_map = ''
		self.height_map = ''
		self.specular_map = ''
		self.light_map = ''
		self.parallax_scale = 0.0
		self.alpha_test = False
		self.alpha_threshold = 0.99
		self.size_bytes = 0

	def load(self, mat):
		self.name = mat.name

		diffuse = mat.diffuse_color
		dif_int = mat.diffuse_intensity
		ambient = mat.ambient
		specCol = mat.specular_color
		spec = mat.specular_intensity
		hardness = mat.specular_hardness

		self.diffuse = [diffuse[0]*dif_int, diffuse[1]*dif_int, diffuse[2]*dif_int, 1.0]
		self.ambient = [ambient, ambient, ambient, 1.0]
		self.specular = [specCol[0]*spec, specCol[1]*spec, specCol[2]*spec, 1.0]
		self.shininess = float(hardness)/4.0

		for tex in mat.texture_slots:
			if tex != None and tex.texture != None and 'image' in dir(tex.texture):
				if tex.texture.image.depth == 32: self.alpha_test = True
				if tex.use_map_normal: self.normal_map = tex.texture.name
				elif tex.use_map_displacement != 0:
					self.height_map = tex.texture.name
					self.parallax_scale = tex.displacement_factor
				elif tex.use_map_color_spec != 0: self.specular_map = tex.texture.name
				else: self.diffuse_map = tex.texture.name

		self.size_bytes = 0

		self.size_bytes += struct.calcsize('<i')	# magic
		self.size_bytes += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')	# name
		self.size_bytes += struct.calcsize('<ffff')*3	# colors
		self.size_bytes += struct.calcsize('<f')	# specular power
		self.size_bytes += struct.calcsize('<B')	# lighting
		self.size_bytes += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')*5	# textures
		self.size_bytes += struct.calcsize('<f')	# parallax scale
		self.size_bytes += struct.calcsize('<Bf')	# alpha test, alpha threshold

		print('Material \"'+self.name+'\" converted')
		print('  size: '+str(self.size_bytes)+' bytes')

	def save(self, f):
		f.write(struct.pack('<i', 179532109))	# write magic
		write_name_to_file(self.name, f)	# write name
		f.write(struct.pack('<ffff', self.diffuse[0], self.diffuse[1], self.diffuse[2], self.diffuse[3]))	# write diffuse color
		f.write(struct.pack('<ffff', self.ambient[0], self.ambient[1], self.ambient[2], self.ambient[3]))	# write ambient color
		f.write(struct.pack('<ffff', self.specular[0], self.specular[1], self.specular[2], self.specular[3]))	# write specular color
		f.write(struct.pack('<f', self.shininess))	# write shininess
		f.write(struct.pack('<B', 1))	# write lighting flag
		write_name_to_file(self.diffuse_map, f)	# write diffuse map
		write_name_to_file(self.normal_map, f)	# write normal map
		write_name_to_file(self.height_map, f)	# write height map
		write_name_to_file(self.specular_map, f)	# write specular map
		write_name_to_file(self.light_map, f)	# write light map
		f.write(struct.pack('<f', self.parallax_scale))	# parallax scale
		#  alpha test
		if self.alpha_test == True: f.write(struct.pack('<B', 1))
		else: f.write(struct.pack('<B', 0))
		f.write(struct.pack('<f', self.alpha_threshold))	# alpha threshold

		print('Material \"'+self.name+'\" saved')

class Vertice:
	def __init__(self):
		self.co = [0.0, 0.0, 0.0]
		self.no = [0.0, 0.0, 0.0]
		self.uv = [0.0, 0.0]
		self.weights = [0.0, 0.0, 0.0, 0.0]
		self.boneids = [-1, -1, -1, -1]
	def __eq__(self, v):
		return self.co == v.co and self.no == v.no and self.uv == v.uv

class Area:
	def __init__(self):
		self.index = []

class Model:
	def __init__(self):
		self.name = ''
		self.verts = []
		self.areas = []
		self.indice_count = 0
		self.size_bytes = 0
		self.weights_and_boneids = False

	def load(self, obj):
		mesh = obj.data

		self.name = mesh.name

		#max_frame = 1
		#if obj.parent != None and obj.parent.action != None:
		#		action = obj.getParent().getAction()
		#		if len(action.getFrameNumbers()) > 0:
		#			frame_numbers = action.getFrameNumbers()
		#			for num in frame_numbers:
		#				if num > max_frame: max_frame = num
		#
		#groups = mesh.getVertGroupNames()
		#if len(groups) > 0:
		#	self.weights_and_boneids = True
		#	if obj.getParent() != None:
		#		parent = obj.getParent()
		#		if parent.getType() == 'Armature':
		#			armdata = parent.getData()
		#			groups = armdata.bones.keys()

		#Blender.Set("curframe", 1)
		#Blender.Window.Redraw()

		fn = 0

		for face in mesh.faces:
			while len(self.areas) < face.material_index+1: self.areas.append(Area())
			face_verts = [None, None, None, None]
			face_indexes = [0, 0, 0, 0]
			for i in range(len(face.vertices)):
				vert = Vertice()
				vert.co = mesh.vertices[face.vertices[i]].co
				if face.use_smooth == 1: vert.no = mesh.vertices[face.vertices[i]].normal
				else: vert.no = face.normal
				#if len(groups) > 0:
				#	vi = mesh.getVertexInfluences(face.v[i].index)
				#	for j in range(len(vi)):
				#		if j > 3: break
				#		vert.weights[j] = vi[j][1]
				#		vert.boneids[j] = groups.index(vi[j][0])
				if mesh.uv_textures.active != None: vert.uv = mesh.uv_textures.active.data[fn].uv[i]
				face_verts[i] = vert
				if i == 3:
					#try:
					#	n = self.verts.index(vert)
					#	self.areas[face.materialIndex].index.append(face_indexes[0])
					#	self.areas[face.materialIndex].index.append(face_indexes[2])
					#	self.areas[face.materialIndex].index.append(n)
					#except:
					self.areas[face.material_index].index.append(face_indexes[0])
					self.areas[face.material_index].index.append(face_indexes[2])
					self.areas[face.material_index].index.append(len(self.verts))
					self.verts.append(vert)
				else:
					#try:
					#	n = self.verts.index(vert)
					#	self.areas[face.materialIndex].index.append(n)
					#except:
					self.areas[face.material_index].index.append(len(self.verts))
					face_indexes[i] = len(self.verts)
					face_verts[i] = vert
					self.verts.append(vert)
			fn = fn + 1

		if len(self.verts) < 3:
			return False

		self.size_bytes = 0

		# magic
		self.size_bytes += struct.calcsize('<i')
		# name
		self.size_bytes += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')
		# header
		self.size_bytes += struct.calcsize('<iiiiBBBB')
		# frames
		self.size_bytes += struct.calcsize('<fff')*len(self.verts)*1
		# areas
		self.indice_count = 0
		for area in self.areas:
			self.size_bytes += struct.calcsize('<i')
			self.size_bytes += struct.calcsize('<I')*len(area.index)
			self.indice_count += len(area.index)
		# normals
		self.size_bytes += struct.calcsize('<fff')*len(self.verts)
		# tex coords
		self.size_bytes += struct.calcsize('<ff')*len(self.verts)
		# weights and boneids
		if self.weights_and_boneids is True:
			self.size_bytes += struct.calcsize('<ffff')*len(self.verts)
			self.size_bytes += struct.calcsize('<hhhh')*len(self.verts)

		print('Mesh \"'+self.name+'\" converted')
		print('  vertices: '+str(len(self.verts)))
		print('  indicles: '+str(self.indice_count))
		print('  areas: '+str(len(self.areas)))
		print('  size: '+str(self.size_bytes)+' bytes')

		return True

	def save(self, f):
		if len(self.verts) < 3: return

		# write magic
		f.write(struct.pack('<i', 179532110))

		# write name
		write_name_to_file(self.name, f)

		# write header
		f.write(struct.pack('<i', len(self.verts)))
		f.write(struct.pack('<i', 1))
		f.write(struct.pack('<i', self.indice_count))
		f.write(struct.pack('<i', len(self.areas)))
		f.write(struct.pack('<B', 255)) # normals
		f.write(struct.pack('<B', 255)) # texcoords
		if self.weights_and_boneids is True:
			f.write(struct.pack('<B', 255)) # weights and bone ids
		else: f.write(struct.pack('<B', 0))
		f.write(struct.pack('<B', 255)) # junk...

		# write the frames
		for v in self.verts:
			f.write(struct.pack('<fff', v.co[0], v.co[1], v.co[2]))

		# write the index
		for area in self.areas:
			f.write(struct.pack('<i', len(area.index)))
			for idx in area.index:
				f.write(struct.pack('<I', idx))

		# write normals
		for v in self.verts:
			f.write(struct.pack('<fff', v.no[0], v.no[1], v.no[2]))

		# write tex coords
		for v in self.verts:
			f.write(struct.pack('<ff', v.uv[0], v.uv[1]))
			
		# write weights and boneids
		if self.weights_and_boneids is True:
			for v in self.verts:
				f.write(struct.pack('<ffff', v.weights[0], v.weights[1], v.weights[2], v.weights[3]))
			for v in self.verts:
				f.write(struct.pack('<hhhh', v.boneids[0], v.boneids[1], v.boneids[2], v.boneids[3]))

		print('Mesh \"'+self.name+'\" saved')

class BezierPoint:
	def __init__(self):
		self.c1 = [0.0, 0.0, 0.0]
		self.p = [0.0, 0.0, 0.0]
		self.c2 = [0.0, 0.0, 0.0]

class Curve:
	def __init__(self):
		self.type = 0
		self.interpolation = 0
		self.points = []

class Ipo:
	def __init__(self):
		self.name = ''
		self.curves = []

def get_ipo_from_ipo(ipo):
	nipo = Ipo()
	for cur in ipo.getCurves():
		curve = Curve()
		if cur.name == "LocX": curve.type = 0
		elif cur.name == "LocY": curve.type = 1
		elif cur.name == "LocZ": curve.type = 2
		elif cur.name == "RotX": curve.type = 3
		elif cur.name == "RotY": curve.type = 4
		elif cur.name == "RotZ": curve.type = 5
		elif cur.name == "ScaleX": curve.type = 6
		elif cur.name == "ScaleY": curve.type = 7
		elif cur.name == "ScaleZ": curve.type = 8
		elif cur.name == "QuatX": curve.type = 9
		elif cur.name == "QuatY": curve.type = 10
		elif cur.name == "QuatZ": curve.type = 11
		elif cur.name == "QuatW": curve.type = 12
		for pnt in cur.getPoints():
			point = BezierPoint()
			point.p[0] = pnt.pt[0]
			point.p[1] = pnt.pt[1]
			if curve.type >= 3 and curve.type <= 5:
				point.p[1] = point.p[1]*10.0
			curve.points.append(point)
		nipo.curves.append(curve)
	return nipo

class Actor(object):
	def __init__(self):
		super(Actor, self).__init__()

		self.name = ''
		self.parent = ''
		self.script = ''

		self.position = [0.0, 0.0, 0.0]
		self.rotation = [0.0, 0.0, 0.0]

		self.physics = 0
		self.shape = 0
		self.bounding_lengths = [1.0, 1.0, 1.0]
		self.bounding_offset = [0.0, 0.0, 0.0]
		self.mass = 0.0
		self.lin_damp = 0.0
		self.ang_damp = 0.0
		self.lin_sleep = 0.8
		self.ang_sleep = 1.0
		self.restitution = 0.0
		self.anis_fric = [1.0, 1.0, 1.0]
		self.lin_factor = [1.0, 1.0, 1.0]
		self.ang_factor = [1.0, 1.0, 1.0]

		self.ipo = None

		self.size_bytes = 0

def get_actor_header_from_object(obj, eobj):
	#Blender.Set("curframe", 1)
	#Blender.Window.Redraw()
	
	eobj.name = obj.name

	if obj.parent != None:
		eobj.parent = obj.parent.name

	try:
		prop = obj.getProperty('script')
		if prop.getType() == 'STRING':
			eobj.script = prop.getData()
	except: pass

	loc = obj.location
	rot = obj.rotation_euler

	eobj.position[0] = loc[0]
	eobj.position[1] = loc[1]
	eobj.position[2] = loc[2]

	eobj.rotation[0] = rot[0]*(180.0/3.1415926535)
	eobj.rotation[1] = rot[1]*(180.0/3.1415926535)
	eobj.rotation[2] = rot[2]*(180.0/3.1415926535)

def write_actor_header(eobj, f):
	# write name
	write_name_to_file(eobj.name, f)
	write_name_to_file(eobj.parent, f)
	write_name_to_file(eobj.script, f)

	# write transformations
	f.write(struct.pack('<fff', eobj.position[0], eobj.position[1], eobj.position[2]))
	f.write(struct.pack('<fff', eobj.rotation[0], eobj.rotation[1], eobj.rotation[2]))
	
	# write ipos
	if eobj.ipo == None:
		f.write(struct.pack('<B', 0))
	else:
		f.write(struct.pack('<B', len(eobj.ipo.curves)))
		for curve in eobj.ipo.curves:
			f.write(struct.pack('<BBi', curve.type, curve.interpolation, len(curve.points)))
			for point in curve.points:
				f.write(struct.pack('<ffffff', point.c1[0], point.c1[1], point.p[0], point.p[1], point.c2[0], point.c2[0]))

	f.write(struct.pack('<B', eobj.physics))
	f.write(struct.pack('<B', eobj.shape))
	f.write(struct.pack('<fff', eobj.bounding_lengths[0], eobj.bounding_lengths[1], eobj.bounding_lengths[2]))
	f.write(struct.pack('<fff', eobj.bounding_offset[0], eobj.bounding_offset[1], eobj.bounding_offset[2]))
	f.write(struct.pack('<f', eobj.mass))
	f.write(struct.pack('<f', eobj.lin_damp))
	f.write(struct.pack('<f', eobj.ang_damp))
	f.write(struct.pack('<f', eobj.lin_sleep))
	f.write(struct.pack('<f', eobj.ang_sleep))
	f.write(struct.pack('<f', eobj.restitution))
	f.write(struct.pack('<fff', eobj.anis_fric[0], eobj.anis_fric[1], eobj.anis_fric[2]))
	f.write(struct.pack('<fff', eobj.lin_factor[0], eobj.lin_factor[1], eobj.lin_factor[2]))
	f.write(struct.pack('<fff', eobj.ang_factor[0], eobj.ang_factor[1], eobj.ang_factor[2]))
	f.write(struct.pack('<i', 0))

def get_actor_header_size(eobj):
	size_bytes = 0
	# name, parent, script
	size_bytes += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')*3
	# transformations

	size_bytes += struct.calcsize('<fff')*2
	# ipo
	size_bytes += struct.calcsize('<B')
	if eobj.ipo != None:
		for curve in eobj.ipo.curves:
			size_bytes += struct.calcsize('<BBi')
			size_bytes += struct.calcsize('<ffffff')*len(curve.points)

	size_bytes += struct.calcsize('<B')	# physics
	size_bytes += struct.calcsize('<B')	# shape
	size_bytes += struct.calcsize('<fff')	# bounding lengths
	size_bytes += struct.calcsize('<fff')	# bounding offsets
	size_bytes += struct.calcsize('<f')	# mass
	size_bytes += struct.calcsize('<f')	# linear damp
	size_bytes += struct.calcsize('<f')	# angular damp
	size_bytes += struct.calcsize('<f')	# linear sleep threshold
	size_bytes += struct.calcsize('<f')	# angular sleep threshold
	size_bytes += struct.calcsize('<f')	# restitution
	size_bytes += struct.calcsize('<fff')	# anisotropic friction
	size_bytes += struct.calcsize('<fff')	# linear factor
	size_bytes += struct.calcsize('<fff')	# angular factor
	size_bytes += struct.calcsize('<i')	# property count

	return size_bytes


class Camera(Actor):
	def __init__(self):
		super(Camera, self).__init__()

		self.fov = 35.0
		self.clip_near = 1.0
		self.clip_far = 1000.0

	def load(self, obj):
		get_actor_header_from_object(obj, self)
		
		data = obj.data

		self.clip_near = data.clip_start
		self.clip_far = data.clip_end
		
		# magic
		self.size_bytes += struct.calcsize('<i')
		# actor specs
		self.size_bytes += get_actor_header_size(self)
		# field of view, clip
		self.size_bytes += struct.calcsize('<fff')

		print('Camera \"'+self.name+'\" converted')
		print('  size: '+str(self.size_bytes)+' bytes')

	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532111))

		write_actor_header(self, f)

		# write field of view
		f.write(struct.pack('<f', self.fov))

		# write clip
		f.write(struct.pack('<ff', self.clip_near, self.clip_far))

		print('Camera \"'+self.name+'\" saved')

class Entity(Actor):
	def __init__(self):
		super(Entity, self).__init__()

		self.bounding_lengths = [0.0, 0.0, 0.0]

		self.scale = [1.0, 1.0, 1.0]
		self.model = ''
		self.armature = ''
		self.materials = []

	def load(self, obj):
		get_actor_header_from_object(obj, self)

		if obj.parent != None:
			if obj.parent.type == 'ARMATURE':
				self.parent = ''
				self.armature = obj.parent.name

		data = obj.data
		scale = obj.scale

		self.scale = scale

		self.model = data.name

		for mat in data.materials:
			self.materials.append(mat.name)

		self.size_bytes += struct.calcsize('<i')	# magic
		self.size_bytes += get_actor_header_size(self)	# actor header
		self.size_bytes += struct.calcsize('<fff')	# scale
		self.size_bytes += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')	# model
		self.size_bytes += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')	# armature
		self.size_bytes += struct.calcsize('<B')	# occluder flag
		
		self.size_bytes += struct.calcsize('<i')	# material count
		self.size_bytes += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')*len(self.materials)	# material names

		print('Entity \"'+self.name+"\" converted")
		print('  model: '+self.model)
		print('  materials: '+str(len(self.materials)))
		print('  size: '+str(self.size_bytes)+' bytes')

	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532112))

		# write actor header
		write_actor_header(self, f)
		
		
		f.write(struct.pack('<fff', self.scale[0], self.scale[1], self.scale[2]))	# write scale
		write_name_to_file(self.model, f)	# write model
		write_name_to_file(self.armature, f)	# write armature
		f.write(struct.pack('<B', 0))	# occluder flag

		# write materials
		f.write(struct.pack('<i', len(self.materials)))
		for mat in self.materials:
			write_name_to_file(mat, f)

		print('Entity \"'+self.name+"\" saved")

class Light(Actor):
	def __init__(self):
		super(Light, self).__init__()

		self.type = 'point'
		self.color = [1.0, 1.0, 1.0, 1.0]
		self.range = 0.0
		self.fade_range = 0.0
		self.inner_cone = 180.0
		self.outer_cone = 0.0
		self.shadow_caster = 1

	def load(self, obj):
		get_actor_header_from_object(obj, self)

		data = obj.data
		col = data.color
		energy = data.energy

		if data.type == "SUN": self.type = 2
		elif data.type == "SPOT": self.type = 3
		else: self.type = 1

		self.color = [col[0]*energy, col[1]*energy, col[2]*energy, 1.0]

		self.range = data.distance
		self.fade_range = data.distance*3

		if data.type == "SPOT":
			self.inner_cone = data.spot_size*(180.0/3.1415926535)/2.0*(1.0-data.spot_blend)
			self.outer_cone = data.spot_size*(180.0/3.1415926535)/2.0-self.inner_cone
		
		self.shadow_map_size = data.shadow_buffer_size
		
		if data.shadow_method == "BUFFER_SHADOW": self.shadow_caster = 1
		else: self.shadow_caster = 0

		self.size_bytes += struct.calcsize('<i')	# magic
		self.size_bytes += get_actor_header_size(self)	# actor header
		self.size_bytes += struct.calcsize('<B')	# type
		self.size_bytes += struct.calcsize('<ffff')	# color
		self.size_bytes += struct.calcsize('<ff')	# range and fade range
		self.size_bytes += struct.calcsize('<ff')	# spot light specs
		self.size_bytes += struct.calcsize('<B')	# shadow flag
		self.size_bytes += struct.calcsize('<Bfff')	# light shaft specs
		
		print('Light \"'+self.name+'\" converted')
		print('  type: '+str(self.type))
		print('  size: '+str(self.size_bytes)+' bytes')

	def save(self, f):
		f.write(struct.pack('<i', 179532113))	# write magic
		write_actor_header(self, f)	# write actor header
		f.write(struct.pack('<B', self.type))	# write type
		f.write(struct.pack('<ffff', self.color[0], self.color[1], self.color[2], self.color[3]))	# write colors
		f.write(struct.pack('<ff', self.range, self.fade_range))	# write range and fade range
		f.write(struct.pack('<ff', self.inner_cone, self.outer_cone))	# write spot light specs
		f.write(struct.pack('<B', self.shadow_caster))	# write shadow flag
		f.write(struct.pack('<Bfff', 0, 1.0, 1.0, 0.0))	# write ligth shaft specs

		print('Light \"'+self.name+'\" saved')

class BoneFrame:
	def __init__(self):
		self.pos = [0.0, 0.0, 0.0]
		self.qua = [0.0, 0.0, 0.0, 1.0]

class Bone:
	def __init__(self):
		self.name = ''
		self.parent = ''
		self.id = 0
		self.pos = [0.0, 0.0, 0.0]
		self.qua = [0.0, 0.0, 0.0, 1.0]
		self.frames = []
		self.size_bytes = 0

class Armature:
	def __init__(self):
		self.name = ''
		self.bones = []
		self.frame_count = 0
		self.ipo = None
		self.size_bytes = 0

	def load(self, obj):
		self.name = obj.name

		data = obj.data

		action = obj.getAction()
		if action != None and len(action.getFrameNumbers()) > 0:
			frame_numbers = action.getFrameNumbers()
			for num in frame_numbers:
				if num > self.frame_count: self.frame_count = num
		
		bone_id = 0
		for name, bone in data.bones.items():
			nbone = Bone()
			nbone.name = name
			if bone.hasParent():
				nbone.parent = bone.parent.name
			nbone.id = bone_id

			mat = Matrix(bone.matrix['ARMATURESPACE'])
			qua = mat.toQuat()
			nbone.pos = [mat[3][0], mat[3][1], mat[3][2]]
			nbone.qua = [qua[1], qua[2], qua[3], qua[0]]
			
			# name and parent
			nbone.size_bytes += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')*2
			# id
			nbone.size_bytes += struct.calcsize('<i')
			# transformations
			nbone.size_bytes += struct.calcsize('<7f')
			# frames
			nbone.size_bytes += struct.calcsize('<7f')*self.frame_count
			
			self.bones.append(nbone)
			bone_id += 1

		for i in range(self.frame_count):
			#Blender.Set("curframe", i+1)

			#Blender.Window.Redraw()
			pose = obj.getPose()
			for bone in self.bones:
				mat = Matrix(pose.bones[bone.name].poseMatrix)
				qua = mat.toQuat()
				frame = BoneFrame()
				frame.pos = [mat[3][0], mat[3][1], mat[3][2]]
				frame.qua = [qua[1], qua[2], qua[3], qua[0]]
				bone.frames.append(frame)
		
		# magic
		self.size_bytes += struct.calcsize('<i')
		# name
		self.size_bytes += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')
		# frame_count
		self.size_bytes += struct.calcsize('<i')
		# bone count
		self.size_bytes += struct.calcsize('<i')
		# bones
		for bone in self.bones:
			self.size_bytes += nbone.size_bytes
		
		print('Armature \"'+self.name+'\" converted')
		print('  Bones: '+str(len(self.bones)))
		print('  size: '+str(self.size_bytes)+' bytes')

	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532122))

		# write actor header
		write_name_to_file(self.name, f)
		# write frame count
		f.write(struct.pack('<i', self.frame_count))
		
		# write bones
		f.write(struct.pack('<i', len(self.bones)))
		for bone in self.bones:
			# name and parent
			write_name_to_file(bone.name, f)
			write_name_to_file(bone.parent, f)
			# id
			f.write(struct.pack('<i', bone.id))
			# transformations
			f.write(struct.pack('<3f', bone.pos[0], bone.pos[1], bone.pos[2]))
			f.write(struct.pack('<4f', bone.qua[0], bone.qua[1], bone.qua[2], bone.qua[3]))
			# frames
			for frame in bone.frames:
				f.write(struct.pack('<3f', frame.pos[0], frame.pos[1], frame.pos[2]))
				f.write(struct.pack('<4f', frame.qua[0], frame.qua[1], frame.qua[2], frame.qua[3]))
		
		print('Armature \"'+self.name+'\" saved')

class Script:
	def __init__(self):
		self.name = ''
		self.text = ''
		self.size_bytes = 0
	
	def load(self, text):
		self.name = text.name
		
		while 1:
			line = text.readline()
			if len(line) < 1: break
			self.text += line
		
		# magic, name
		self.size_bytes = struct.calcsize('<i')
		self.size_bytes += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')
		# text length, text
		self.size_bytes += struct.calcsize('<i')
		self.size_bytes += len(self.text)
		
		print('Script: '+text.name+' converted')
	
	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532121))
		# write name
		write_name_to_file(self.name, f)
		# write text
		f.write(struct.pack('<i', len(self.text)))
		if len(self.text) > 0: f.write(struct.pack('<'+str(len(self.text))+'s', self.text))
		
		print('Script: '+self.name+' saved')

class Scene:
	def __init__(self):
		self.name = ''
		self.ambient = [0.0, 0.0, 0.0, 1.0]
		self.size_bytes = 0

	def load(self, scn):
		self.name = scn.name

		self.ambient[0] = scn.world.ambient_color.r
		self.ambient[1] = scn.world.ambient_color.g
		self.ambient[2] = scn.world.ambient_color.b

		# magic
		self.size_bytes += struct.calcsize('<i', )
		#name
		self.size_bytes += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')
		# ambience
		self.size_bytes += struct.calcsize('<ffff')

	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532120))
		# write name
		write_name_to_file(self.name, f)
		# write ambience
		f.write(struct.pack('<ffff', self.ambient[0], self.ambient[1], self.ambient[2], self.ambient[3]))

def export(path, sce):
	print()
	print('##############################')
	print('#### elf pak exporter 1.0 ####')
	print('##############################')
	print()

	scripts = []
	script_names = []
	textures = []
	texture_names = []
	materials = []
	material_names = []
	models = []
	model_names = []
	cameras = []
	entities = []
	armatures = []
	lights = []
	scenes = []

	cur_scene = Scene()
	cur_scene.load(sce)
	scenes.append(cur_scene)
	
	for text in bpy.data.texts:
		if '.lua' in text.name:
			script = Script()
			script.load(text)
			scripts.append(script)

	for obj in sce.objects:
		if obj.name[0] == '.': continue
		if obj.type == 'MESH':
			entity = Entity()
			entity.load(obj)
			entities.append(entity)
			data = obj.data
			if data.name not in model_names:
				model = Model()
				if model.load(obj) == False: continue
				models.append(model)
				model_names.append(model.name)
				for mat in data.materials:
					if mat.name not in material_names:
						material_names.append(mat.name)
						material = Material()
						material.load(mat)
						materials.append(material)
						for tex in mat.texture_slots:
							if tex != None and tex.texture != None and 'image' in dir(tex.texture) and tex.texture.name not in texture_names:
								texture = Texture()
								if texture.load(tex) == True:
									textures.append(texture)
									texture_names.append(texture.name)
		elif obj.type == 'LAMP':
			light = Light()
			light.load(obj)
			lights.append(light)
		elif obj.type == 'CAMERA':
			camera = Camera()
			camera.load(obj)
			cameras.append(camera)
		elif obj.type == 'ARMATURE':
			armature = Armature()
			armature.load(obj)
			armatures.append(armature)

	print()
	print()

	offset = 0

	# calculate the index offset
	offset += struct.calcsize('<B')
	offset += struct.calcsize('<'+str(ELF_NAME_LENGTH)+'s')
	offset += struct.calcsize('<i')
	offset *= len(scenes)+len(scripts)+len(textures)+len(materials)+len(models)+len(cameras)+len(entities)+len(lights)+len(armatures)
	# magic, version and number of indexes
	offset += struct.calcsize('<i')
	offset += struct.calcsize('<i')
	offset += struct.calcsize('<i')

	f = open(path, 'wb')

	# magic
	f.write(struct.pack('<i', 179532100))
	# version
	f.write(struct.pack('<i', ELF_PAK_VERSION))

	# index count
	f.write(struct.pack('<i', len(scenes)+len(scripts)+len(textures)+len(materials)+len(models)+len(cameras)+len(entities)+len(lights)+len(armatures)))

	# write the index
	for scene in scenes:
		f.write(struct.pack('<B', 6))
		write_name_to_file(scene.name, f)
		f.write(struct.pack('<i', offset))
		offset += scene.size_bytes
	for script in scripts:
		f.write(struct.pack('<B', 17))
		write_name_to_file(script.name, f)
		f.write(struct.pack('<i', offset))
		offset += script.size_bytes
	for texture in textures:
		f.write(struct.pack('<B', 0))
		write_name_to_file(texture.name, f)
		f.write(struct.pack('<i', offset))
		offset += texture.size_bytes
	for material in materials:
		f.write(struct.pack('<B', 1))
		write_name_to_file(material.name, f)
		f.write(struct.pack('<i', offset))
		offset += material.size_bytes
	for model in models:
		f.write(struct.pack('<B', 2))
		write_name_to_file(model.name, f)
		f.write(struct.pack('<i', offset))
		offset += model.size_bytes
	for camera in cameras:
		f.write(struct.pack('<B', 3))
		write_name_to_file(camera.name, f)
		f.write(struct.pack('<i', offset))
		offset += camera.size_bytes
	for entity in entities:
		f.write(struct.pack('<B', 4))
		write_name_to_file(entity.name, f)
		f.write(struct.pack('<i', offset))
		offset += entity.size_bytes
	for light in lights:
		f.write(struct.pack('<B', 5))
		write_name_to_file(light.name, f)
		f.write(struct.pack('<i', offset))
		offset += light.size_bytes
	for armature in armatures:
		f.write(struct.pack('<B', 22))
		write_name_to_file(armature.name, f)
		f.write(struct.pack('<i', offset))
		offset += armature.size_bytes

	# write the data
	for scene in scenes:
		scene.save(f)
	for script in scripts:
		script.save(f)
	for texture in textures:
		texture.save(f)
	for material in materials:
		material.save(f)
	for model in models:
		model.save(f)
	for camera in cameras:
		camera.save(f)
	for entity in entities:
		entity.save(f)
	for light in lights:
		light.save(f)
	for armature in armatures:
		armature.save(f)

	f.close()

	print()
	print('######### EXPORT DONE #########')
	print()
	print()

from bpy.props import *

class ExportPAK(bpy.types.Operator):
	"""Export to the BlendELF (.pak) format"""
	bl_idname = "export.pak"
	bl_label = "Export PAK"

	filepath = StringProperty()
	filename_ext = ".pak"
	
	def execute(self, context):
		export(self.filepath, context.scene)
		return {'FINISHED'}
	
	def invoke(self, context, event):	
		wm = context.window_manager
		wm.add_fileselect(self)
		return {'RUNNING_MODAL'}

def menu_func(self, context):

    default_path = os.path.splitext(bpy.data.filepath)[0] + ".pak"
    self.layout.operator(ExportPAK.bl_idname, text="BlendELF (.pak)").filepath = default_path

def register():
    bpy.types.INFO_MT_file_export.append(menu_func)



def unregister():
    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()

