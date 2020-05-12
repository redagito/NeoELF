
function ediInitWidCamera()
	editor.gui.properties.widCamera = {}
	local widCamera = editor.gui.properties.widCamera

	widCamera.object = CreateScreen(editor.gui.properties.object, "WidCamera", 0, 25, 248, 86)

	widCamera.fovAttr = ediCreateFloatGroupAttribute(widCamera.object, "Fov", 4, 22, 0, 179, "", 1, "SetCameraFov")
	widCamera.clipAttr = ediCreateFloatGroupAttribute(widCamera.object, "Clip", 4, 42, 0, nil, "", 2, "SetCameraClip")

	widCamera.copyToViewBut = CreateButton(widCamera.object, "CopyToViewBut", 10, 64, 228, 18, "Copy To View")

	widCamera.minimize = CreateButton(widCamera.object, "Camera", 1, 1, 246, 16, "Camera")

	SetScreenSize(widCamera.object, 248, 16)
end

function ediUpdateWidCameraSelection()
	local widCamera = editor.gui.properties.widCamera
	local sel = ediGetSelection()

	local clip = GetCameraClip(sel)

	ediSetFloatGroupAttributeValues(widCamera.fovAttr, {GetCameraFov(sel)})
	ediSetFloatGroupAttributeValues(widCamera.clipAttr, {clip.x, clip.y})
end

function ediUpdateWidCamera()
	local widCamera = editor.gui.properties.widCamera
	local sel = ediGetSelection()

	if GetGuiObjectEvent(widCamera.minimize) == CLICKED then
		if GetGuiObjectSize(widCamera.object).y > 16 then
			SetScreenSize(widCamera.object, 248, 16)
		else
			SetScreenSize(widCamera.object, 248, 86)
		end
		ediPackScreensVer(editor.gui.properties, 25)
	end

	ediUpdateAttribute(widCamera.fovAttr, sel)
	ediUpdateAttribute(widCamera.clipAttr, sel)

	if GetGuiObjectEvent(widCamera.copyToViewBut) == CLICKED then
		local clip = GetCameraClip(sel)
		local pos = GetActorPosition(sel)
		local rot = GetActorRotation(sel)
		SetActorPosition(editor.scene.camera.object, pos.x, pos.y, pos.z)
		SetActorRotation(editor.scene.camera.object, rot.x, rot.y, rot.z)
		SetCameraFov(editor.scene.camera.object, GetCameraFov(sel))
		SetCameraAspect(editor.scene.camera.object, GetCameraAspect(sel))
		SetCameraClip(editor.scene.camera.object, clip.x, clip.y)
	end
end

