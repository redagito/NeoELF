
function ediInitWidLight()
	editor.gui.properties.widLight = {}
	local widLight = editor.gui.properties.widLight

	widLight.object = CreateScreen(editor.gui.properties.object, "WidLight", 0, 25, 248, 121)

	widLight.colorAttr = ediCreateFloatGroupAttribute(widLight.object, "Color", 4, 42, 0, nil, "", 4, "SetLightColor")
	widLight.distanceAttr = ediCreateFloatGroupAttribute(widLight.object, "Range", 4, 62, 0.001, nil, "", 2, "SetLightRange")
	widLight.spotConeAttr = ediCreateFloatGroupAttribute(widLight.object, "Cone", 4, 82, 0, nil, "", 2, "SetLightCone")
	widLight.shadowsAttr = ediCreateBooleanAttribute(widLight.object, "Shadows", 4, 102, false, "SetLightShadows")

	widLight.typeLab = CreateLabel(widLight.object, "TypeLab", 4, 26, "Type")

	SetGuiObjectColor(widLight.typeLab, 1.0, 1.0, 1.0, 0.6)

	widLight.typePoint = CreateButton(widLight.object, "TypePoint", 80, 22, 54, 18, "Point")
	widLight.typeSun = CreateButton(widLight.object, "TypeSun", 134, 22, 55, 18, "Sun")
	widLight.typeSpot = CreateButton(widLight.object, "TypeSpot", 189, 22, 55, 18, "Spot")

	widLight.minimize = CreateButton(widLight.object, "Light", 1, 1, 246, 14, "Light")

	SetScreenSize(widLight.object, 248, 16)
end

function ediUpdateWidLightSelection()
	local widLight = editor.gui.properties.widLight
	local sel = ediGetSelection()
	local col = GetLightColor(sel)

	ediSetFloatGroupAttributeValues(widLight.colorAttr, {col.r, col.g, col.b, col.a})
	ediSetFloatGroupAttributeValues(widLight.distanceAttr, {GetLightRange(sel), GetLightFadeRange(sel)})
	ediSetFloatGroupAttributeValues(widLight.spotConeAttr, {GetLightInnerCone(sel), GetLightOuterCone(sel)})
	ediSetBooleanAttributeValue(widLight.shadowsAttr, GetLightShadows(sel))

	SetGuiObjectActive(widLight.typePoint, true)
	SetGuiObjectActive(widLight.typeSun, true)
	SetGuiObjectActive(widLight.typeSpot, true)

	if GetLightType(sel) == POINT_LIGHT then SetGuiObjectActive(widLight.typePoint, false)
	elseif GetLightType(sel) == SUN_LIGHT then SetGuiObjectActive(widLight.typeSun, false)
	elseif GetLightType(sel) == SPOT_LIGHT then SetGuiObjectActive(widLight.typeSpot, false) end
end

function ediUpdateWidLight()
	local widLight = editor.gui.properties.widLight
	local sel = ediGetSelection()

	if GetGuiObjectEvent(widLight.minimize) == CLICKED then
		if GetGuiObjectSize(widLight.object).y > 16 then
			SetScreenSize(widLight.object, 248, 16)
		else
			SetScreenSize(widLight.object, 248, 121)
		end
		ediPackScreensVer(editor.gui.properties, 25)
	end

	if GetGuiObjectEvent(widLight.typePoint) == CLICKED then
		SetLightType(sel, POINT_LIGHT)
		SetGuiObjectActive(widLight.typePoint, false)
		SetGuiObjectActive(widLight.typeSun, true)
		SetGuiObjectActive(widLight.typeSpot, true)
	end
	if GetGuiObjectEvent(widLight.typeSun) == CLICKED then
		SetLightType(sel, SUN_LIGHT)
		SetGuiObjectActive(widLight.typePoint, true)
		SetGuiObjectActive(widLight.typeSun, false)
		SetGuiObjectActive(widLight.typeSpot, true)
	end
	if GetGuiObjectEvent(widLight.typeSpot) == CLICKED then
		SetLightType(sel, SPOT_LIGHT)
		SetGuiObjectActive(widLight.typePoint, true)
		SetGuiObjectActive(widLight.typeSun, true)
		SetGuiObjectActive(widLight.typeSpot, false)
	end

	ediUpdateAttribute(widLight.colorAttr, sel)
	ediUpdateAttribute(widLight.distanceAttr, sel)
	ediUpdateAttribute(widLight.spotConeAttr, sel)
	ediUpdateAttribute(widLight.shadowsAttr, sel)
end

