
function ediInitWidShaft()
	editor.gui.properties.widShaft = {}
	local widShaft = editor.gui.properties.widShaft

	widShaft.object = CreateScreen(editor.gui.properties.object, "WidShaft", 0, 25, 248, 109)

	widShaft.enabledAttr = ediCreateBooleanAttribute(widShaft.object, "Enabled", 4, 22, false, "SetLightShaft")
	widShaft.sizeAttr = ediCreateFloatGroupAttribute(widShaft.object, "Size", 4, 42, 0, nil, "", 1, "SetLightShaftSize")
	widShaft.intensityAttr = ediCreateFloatGroupAttribute(widShaft.object, "Intensity", 4, 62, 0, nil, "", 1, "SetLightShaftIntensity")
	widShaft.fadeOffAttr = ediCreateFloatGroupAttribute(widShaft.object, "Fade Off", 4, 82, 0, nil, "", 1, "SetLightShaftFadeOff")

	widShaft.minimize = CreateButton(widShaft.object, "Shaft", 1, 1, 246, 14, "Shaft")

	SetScreenSize(widShaft.object, 248, 16)
end

function ediUpdateWidShaftSelection()
	local widShaft = editor.gui.properties.widShaft
	local sel = ediGetSelection()

	ediSetBooleanAttributeValue(widShaft.enabledAttr, GetLightShaft(sel))
	ediSetFloatGroupAttributeValues(widShaft.sizeAttr, {GetLightShaftSize(sel)})
	ediSetFloatGroupAttributeValues(widShaft.intensityAttr, {GetLightShaftIntensity(sel)})
	ediSetFloatGroupAttributeValues(widShaft.fadeOffAttr, {GetLightShaftFadeOff(sel)})
end

function ediUpdateWidShaft()
	local widShaft = editor.gui.properties.widShaft
	local sel = ediGetSelection()

	if GetGuiObjectEvent(widShaft.minimize) == CLICKED then
		if GetGuiObjectSize(widShaft.object).y > 16 then
			SetScreenSize(widShaft.object, 248, 16)
		else
			SetScreenSize(widShaft.object, 248, 109)
		end
		ediPackScreensVer(editor.gui.properties, 25)
	end

	ediUpdateAttribute(widShaft.enabledAttr, sel)
	ediUpdateAttribute(widShaft.sizeAttr, sel)
	ediUpdateAttribute(widShaft.intensityAttr, sel)
	ediUpdateAttribute(widShaft.fadeOffAttr, sel)
end

