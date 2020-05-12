
function ediInitCamera()
	editor.scene.camera = {}
	local camera = editor.scene.camera

	camera.moveSpeed = 20.0
	camera.irot = {}
	camera.irot.x = 0
	camera.irot.y = 0
	camera.prevMouseWheel = 0

	camera.object = CreateCamera("EditorELF_Camera")

	ediAddCamera()
	ediResetCamera()

	SetActorRotation(camera.object, 90.0, 0.0, 0.0)
	SetActorPosition(camera.object, 0.0, -10.0, 0.0)
end

function ediResetCamera()
	local camera = editor.scene.camera

	local cam = GetSceneCameraByIndex(editor.scene.object, 0)
	if cam ~= nil then
		local pos = GetActorPosition(cam)
		local rot = GetActorRotation(cam)
		local fov = GetCameraFov(cam)
		local aspect = GetCameraAspect(cam)
		local clip = GetCameraClip(cam)
		SetActorPosition(camera.object, pos.x, pos.y, pos.z)
		SetActorRotation(camera.object, rot.x, rot.y, rot.z)
		SetCameraFov(camera.object, fov)
		SetCameraAspect(camera.object, aspect)
		SetCameraClip(camera.object, clip.x, clip.y)
	else
		SetCameraFov(camera.object, 35.0)
		SetCameraAspect(camera.object, -1.0)
		SetCameraClip(camera.object, 1.0, 250.0)
	end

end

function ediAddCamera()
	AddSceneCamera(editor.scene.object, editor.scene.camera.object)
	SetSceneActiveCamera(editor.scene.object, editor.scene.camera.object)
end

function ediRemoveCamera()
	RemoveSceneCameraByObject(editor.scene.object, editor.scene.camera.object)
end

function ediUpdateCamera()
	local camera = editor.scene.camera
	local moveSpeed = camera.moveSpeed

	if GetKeyState(KEY_LSHIFT) == DOWN then moveSpeed = moveSpeed * 3 end
	if GetKeyState(KEY_LALT) == DOWN then moveSpeed = moveSpeed * 0.3 end

	if GetGuiActiveTextField(editor.gui.object) == nil then
		if GetKeyState(KEY_W) ~= UP then MoveActorLocal(editor.scene.camera.object, 0.0, 0.0, -moveSpeed) end
		if GetKeyState(KEY_S) ~= UP then MoveActorLocal(editor.scene.camera.object, 0.0, 0.0, moveSpeed) end
		if GetKeyState(KEY_A) ~= UP then MoveActorLocal(editor.scene.camera.object, -moveSpeed, 0.0, 0.0) end
		if GetKeyState(KEY_D) ~= UP and GetKeyState(KEY_LCTRL) == UP then MoveActorLocal(camera.object, moveSpeed, 0.0, 0.0) end
	end

	if GetMouseButtonState(BUTTON_RIGHT) ~= UP then
		local mf = GetMouseForce()

		if AboutZero(mf.x) ~= true then camera.irot.x = -mf.x*20.0 end
		if AboutZero(mf.y) ~= true then camera.irot.y = -mf.y*20.0 end

		RotateActor(camera.object, 0.0, 0.0, camera.irot.x)
		RotateActorLocal(camera.object, camera.irot.y, 0.0, 0.0)

		SetMousePosition(GetWindowWidth()/2, GetWindowHeight()/2)
	end

	if GetGuiTrace(editor.gui.object) == nil then
		local diff = GetMouseWheel()-camera.prevMouseWheel
		if diff ~= 0 then
			MoveActorLocal(editor.scene.camera.object, 0, 0, -moveSpeed*diff*15)
		end
	end

	camera.irot.x = editor.scene.camera.irot.x*0.5
	camera.irot.y = editor.scene.camera.irot.y*0.5
	camera.prevMouseWheel = GetMouseWheel()
end

