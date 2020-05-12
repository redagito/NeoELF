
EDI_NONE = 0
EDI_MOVE = 1
EDI_ROTATE = 2

function ediInitScene()
	editor.scene = {}
	local scene = editor.scene

	scene.action = {}
	scene.action.type = EDI_NONE

	scene.object = CreateScene("EditorELF_Scene")
	SetScenePhysics(scene.object, false)
	SetSceneRunScripts(scene.object, false)
	SetScene(scene.object)

	ediInitCamera()
end

function ediTraceSceneSelection()
	local scene = editor.scene

	if GetGuiTrace(editor.gui.object) ~= nil then return nil end

	local raystart = GetActorPosition(scene.camera.object)

	local mousePos = GetMousePosition()
	local wwidth = GetWindowWidth()
	local wheight = GetWindowHeight()
	local clip = GetCameraClip(scene.camera.object)
	local fpsize = GetCameraFarPlaneSize(scene.camera.object)

	local rayend = CreateVec3f(0.0, 0.0, 0.0)
	rayend.x = mousePos.x/wwidth*fpsize.x-fpsize.x/2
	rayend.y = (wheight-mousePos.y)/wheight*fpsize.y-fpsize.y/2
	rayend.z = -clip.y

	local orient = GetActorOrientation(scene.camera.object)
	rayend = MulQuaVec3f(orient, rayend)
	rayend = AddVec3fVec3f(raystart, rayend)

	local col = GetDebugSceneRayCastResult(scene.object, raystart.x, raystart.y, raystart.z, rayend.x, rayend.y, rayend.z)
	if col ~= nil then
		return GetCollisionActor(col)
	end

	return nil
end

function ediSetSelection(act)
	local scene = editor.scene

	if scene.selection ~= nil then
			SetActorSelected(scene.selection, false)
	end

	scene.selection = act

	if scene.selection ~= nil then
		SetActorSelected(scene.selection, true)
		ediUpdatePropertiesSelection()
	else
		ediSetPropertiesTab(EDI_MENU)
	end
end

function ediGetSelection()
	return editor.scene.selection
end

function ediOpenScene(path)
	local scn = CreateSceneFromFile("", path)
	if scn ~= nil then
		ediSetSelection(nil)
		editor.scene.object = scn
		SetScenePhysics(editor.scene.object, false)
		SetSceneRunScripts(editor.scene.object, false)
		SetScene(editor.scene.object)
		ediAddCamera()
		ediResetCamera()
		return true
	end
	return false
end

function ediSaveScene(path)
	ediRemoveCamera()
	local status = SaveScene(editor.scene.object, path)
	ediAddCamera()
	return status
end

function ediImportScene(path)
	local scn = CreateSceneFromFile("", path)
	if scn ~= nil then
		while GetSceneCameraCount(scn) > 0 do
			cam = GetSceneCameraByIndex(scn, 0)
			AddSceneCamera(editor.scene.object, cam)
		end
		while GetSceneEntityCount(scn) > 0 do
			ent = GetSceneEntityByIndex(scn, 0)
			AddSceneEntity(editor.scene.object, ent)
		end
		while GetSceneLightCount(scn) > 0 do
			lig = GetSceneLightByIndex(scn, 0)
			AddSceneLight(editor.scene.object, lig)
		end
		while GetSceneParticlesCount(scn) > 0 do
			par = GetSceneParticlesByIndex(scn, 0)
			AddSceneParticles(editor.scene.object, par)
		end
		while GetSceneSpriteCount(scn) > 0 do
			spr = GetSceneSpritesByIndex(scn, 0)
			AddSceneSprites(editor.scene.object, spr)
		end

		return true
	end

	return false
end

function ediUpdateScene()
	local scene = editor.scene

	ediUpdateCamera()

	if GetMouseButtonState(BUTTON_LEFT) == PRESSED then
		if GetGuiTrace(editor.gui.object) == nil and
			scene.action.type == EDI_NONE then
			act = ediTraceSceneSelection()
			if act ~= nil then ediSetSelection(act) end
		end

		scene.action.type = EDI_NONE
	end

	if scene.selection ~= nil then
		if GetGuiActiveTextField(editor.gui.object) == nil then
			if GetKeyState(KEY_G) == PRESSED then
				scene.action.origPos = GetActorPosition(scene.selection)
				scene.action.type = EDI_MOVE
			elseif GetKeyState(KEY_R) == PRESSED then
				scene.action.origRot = GetActorRotation(scene.selection)
				scene.action.type = EDI_ROTATE
			end
		end

		if GetMouseButtonState(BUTTON_RIGHT) == UP then
			if scene.action.type == EDI_MOVE then
				local pos = GetActorPosition(scene.selection)
				local camOrient = GetActorOrientation(scene.camera.object)
				local camPos = GetActorPosition(scene.camera.object)

				local speed = GetVec3fLength(SubVec3fVec3f(pos, camPos))*0.001

				local mf = GetMouseForce()
				local offset = CreateVec3f(mf.x*speed, -mf.y*speed, 0.0)

				offset = MulQuaVec3f(camOrient, offset)
				local newPos = AddVec3fVec3f(pos, offset)

				SetActorPosition(scene.selection, newPos.x, newPos.y, newPos.z)

				ediUpdateWidActorSelection()
			elseif scene.action.type == EDI_ROTATE then
				local orient = GetActorOrientation(scene.selection)
				local invOrient = GetQuaInverted(orient)
				local camOrient = GetActorOrientation(scene.camera.object)

				local axis = CreateVec3f(0.0, 0.0, -1.0)
				local axis = MulQuaVec3f(camOrient, axis)
				local axis = MulQuaVec3f(invOrient, axis)
				local mf = GetMouseForce()

				local offset = CreateQuaFromAngleAxis(mf.y, axis.x, axis.y, axis.z)
				local newOrient = MulQuaQua(orient, offset)

				SetActorOrientation(scene.selection, newOrient.x, newOrient.y, newOrient.z, newOrient.w)

				ediUpdateWidActorSelection()
			end
		end

		if GetKeyState(KEY_DEL) == PRESSED and GetGuiActiveTextField(editor.gui.object) == nil then
			RemoveSceneActorByObject(scene.object, scene.selection)
			ediSetSelection(nil)
		end

		if GetKeyState(KEY_LCTRL) == DOWN and GetKeyState(KEY_D) == PRESSED then
			local act = nil
			if GetObjectType(scene.selection) == ENTITY then
				act = CreateEntity("Entity")

				local mdl = GetEntityModel(scene.selection)
				if mdl ~= nil then
					SetEntityModel(act, mdl)
				end

				for i=0, GetEntityMaterialCount(scene.selection)-1 do
					local mat = GetEntityMaterial(scene.selection, i)
					SetEntityMaterial(act, i, mat)
				end

				local lengths = GetActorBoundingLengths(scene.selection)
				local offset = GetActorBoundingOffset(scene.selection)
				local anisFric = GetActorAnisotropicFriction(scene.selection)
				local linFactor = GetActorLinearFactor(scene.selection)
				local angFactor = GetActorAngularFactor(scene.selection)

				SetActorShape(act, GetActorShape(act))
				SetActorBoundingLengths(act, lengths.x, lengths.y, lengths.z)
				SetActorBoundingOffset(act, offset.x, offset.y, offset.z)
				SetActorDamping(act, GetActorLinearDamping(scene.selection),
					GetActorAngularDamping(scene.selection))
				SetActorMass(act, GetActorMass(act))
				SetActorSleep(act, GetActorLinearSleep(scene.selection),
					GetActorAngularSleep(scene.selection))
				SetActorRestitution(act, GetActorRestitution(scene.selection))
				SetActorAnisotropicFriction(act, anisFric.x, anisFric.y, anisFric.z)
				SetActorLinearFactor(act, angFactor.x, angFactor.y, angFactor.z)
				SetActorAngularFactor(act, angFactor.x, angFactor.y, angFactor.z)
				SetActorPhysics(act, GetActorPhysics(scene.selection))

				AddSceneEntity(scene.object, act)
			end

			if act ~= nil then
				local pos = GetActorPosition(scene.selection)
				local rot = GetActorRotation(scene.selection)
				SetActorPosition(act, pos.x, pos.y, pos.z)
				SetActorRotation(act, rot.x, rot.y, rot.z)

				ediSetSelection(act)

				scene.action.origPos = GetActorPosition(scene.selection)
				scene.action.type = EDI_MOVE
			end
		end

		if GetMouseButtonState(BUTTON_RIGHT) == PRESSED then
			if scene.action.type == EDI_MOVE then
				SetActorPosition(editor.scene.selection,
					scene.action.origPos.x,
					scene.action.origPos.y,
					scene.action.origPos.z)
				scene.action.type = EDI_NONE
				ediUpdateWidActorSelection()
			elseif scene.action.type == EDI_ROTATE then
				SetActorRotation(editor.scene.selection,
					scene.action.origRot.x,
					scene.action.origRot.y,
					scene.action.origRot.z)
				scene.action.type = EDI_NONE
				ediUpdateWidActorSelection()
			end
		end
	end
end

