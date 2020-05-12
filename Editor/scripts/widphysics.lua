
function ediInitWidPhysics()
	editor.gui.properties.widPhysics = {}
	local widPhysics = editor.gui.properties.widPhysics

	widPhysics.prevMouseWheel = 0

	widPhysics.object = CreateScreen(editor.gui.properties.object, "WidPhysics", 0, 25, 248, 277)

	widPhysics.enabledAttr = ediCreateBooleanAttribute(widPhysics.object, "Enabled", 4, 22, false, "SetActorPhysics")
	widPhysics.lengthsAttr = ediCreateFloatGroupAttribute(widPhysics.object, "Lengths", 4, 42, 0, nil, "", 3, "SetActorBoundingLengths")
	widPhysics.offsetAttr = ediCreateFloatGroupAttribute(widPhysics.object, "Offset", 4, 62, 0, nil, "", 3, "SetActorBoundingOffset")
	widPhysics.massAttr = ediCreateFloatGroupAttribute(widPhysics.object, "Mass", 4, 82, 0, nil, "", 1, "SetActorMass")
	widPhysics.dampAttr = ediCreateFloatGroupAttribute(widPhysics.object, "Damp Li/An", 4, 102, 0, nil, "", 2, "SetActorDamping")
	widPhysics.sleepAttr = ediCreateFloatGroupAttribute(widPhysics.object, "Sleep Li/An", 4, 122, 0, nil, "", 2, "SetActorSleep")
	widPhysics.restituAttr = ediCreateFloatGroupAttribute(widPhysics.object, "Restitu", 4, 142, 0, nil, "", 1, "SetActorRestitution")
	widPhysics.anisFricAttr = ediCreateFloatGroupAttribute(widPhysics.object, "AnisFric", 4, 162, 0, nil, "", 3, "SetActorAnisotropicFriction")
	widPhysics.liFactorAttr = ediCreateFloatGroupAttribute(widPhysics.object, "LiFactor", 4, 182, 0, nil, "", 3, "SetActorLinearFactor")
	widPhysics.anFactorAttr = ediCreateFloatGroupAttribute(widPhysics.object, "AnFactor", 4, 202, 0, nil, "", 3, "SetActorAngularFactor")

	widPhysics.shapeLab = CreateLabel(widPhysics.object, "ShapeLab", 4, 226, "Shape")
	SetGuiObjectColor(widPhysics.shapeLab, 1.0, 1.0, 1.0, 0.6)

	widPhysics.shapeTxl = CreateTextList(widPhysics.object, "Shape", 80, 224, 3, 154)
	AddTextListItem(widPhysics.shapeTxl, " Box")
	AddTextListItem(widPhysics.shapeTxl, " Sphere")
	AddTextListItem(widPhysics.shapeTxl, " Mesh")
	AddTextListItem(widPhysics.shapeTxl, " CapsuleX")
	AddTextListItem(widPhysics.shapeTxl, " CapsuleY")
	AddTextListItem(widPhysics.shapeTxl, " CapsuleZ")
	AddTextListItem(widPhysics.shapeTxl, " ConeX")
	AddTextListItem(widPhysics.shapeTxl, " ConeY")
	AddTextListItem(widPhysics.shapeTxl, " ConeZ")

	widPhysics.shapeScrollBar = CreateSlider(widPhysics.object, "ShapeScrollBar", 238, 228, 0, 37, 1.0)

	widPhysics.minimize = CreateButton(widPhysics.object, "Physics", 1, 1, 246, 14, "Physics")

	SetScreenSize(widPhysics.object, 248, 16)
end

function ediUpdateWidPhysicsSelection()
	local widPhysics = editor.gui.properties.widPhysics

	local sel = ediGetSelection()
	local lengths = GetActorBoundingLengths(sel)
	local offset = GetActorBoundingOffset(sel)
	local anisFric = GetActorAnisotropicFriction(sel)
	local liFactor = GetActorLinearFactor(sel)
	local anFactor = GetActorAngularFactor(sel)

	ediSetBooleanAttributeValue(widPhysics.enabledAttr, GetActorPhysics(sel))
	ediSetFloatGroupAttributeValues(widPhysics.lengthsAttr, {lengths.x, lengths.y, lengths.z})
	ediSetFloatGroupAttributeValues(widPhysics.offsetAttr, {offset.x, offset.y, offset.z})
	ediSetFloatGroupAttributeValues(widPhysics.massAttr, {GetActorMass(sel)})
	ediSetFloatGroupAttributeValues(widPhysics.dampAttr, {GetActorLinearDamping(sel), GetActorAngularDamping(sel)})
	ediSetFloatGroupAttributeValues(widPhysics.sleepAttr, {GetActorLinearSleep(sel), GetActorAngularSleep(sel)})
	ediSetFloatGroupAttributeValues(widPhysics.restituAttr, {GetActorRestitution(sel)})
	ediSetFloatGroupAttributeValues(widPhysics.anisFricAttr, {anisFric.x, anisFric.y, anisFric.z})
	ediSetFloatGroupAttributeValues(widPhysics.liFactorAttr, {liFactor.x, liFactor.y, liFactor.z})
	ediSetFloatGroupAttributeValues(widPhysics.anFactorAttr, {anFactor.x, anFactor.y, anFactor.z})

	SetTextListSelection(widPhysics.shapeTxl, GetActorShape(sel))
end

function ediUpdateWidPhysics()
	local widPhysics = editor.gui.properties.widPhysics
	local sel = ediGetSelection()

	if GetGuiObjectEvent(widPhysics.minimize) == CLICKED then
		if GetGuiObjectSize(widPhysics.object).y > 16 then
			SetScreenSize(widPhysics.object, 248, 16)
		else
			SetScreenSize(widPhysics.object, 248, 277)
		end
		ediPackScreensVer(editor.gui.properties, 25)
	end

	ediUpdateAttribute(widPhysics.enabledAttr, sel)
	ediUpdateAttribute(widPhysics.lengthsAttr, sel)
	ediUpdateAttribute(widPhysics.offsetAttr, sel)
	ediUpdateAttribute(widPhysics.massAttr, sel)
	ediUpdateAttribute(widPhysics.dampAttr, sel)
	ediUpdateAttribute(widPhysics.sleepAttr, sel)
	ediUpdateAttribute(widPhysics.restituAttr, sel)
	ediUpdateAttribute(widPhysics.anisFricAttr, sel)
	ediUpdateAttribute(widPhysics.liFactorAttr, sel)
	ediUpdateAttribute(widPhysics.anFactorAttr, sel)

	if GetGuiObjectEvent(widPhysics.shapeTxl) == SELECTION_CHANGED then
		SetActorShape(sel, GetTextListSelectionIndex(widPhysics.shapeTxl))
	end

	local diff = GetTextListItemCount(widPhysics.shapeTxl)- GetTextListRowCount(widPhysics.shapeTxl)
	if diff > 0 then
		if GetGuiObjectEvent(widPhysics.shapeScrollBar) == VALUE_CHANGED then
			SetTextListOffset(widPhysics.shapeTxl, diff*(1.0-GetSliderValue(widPhysics.shapeScrollBar)))
		end

		if GetGuiTrace(editor.gui.object) ~= nil and
			GetGuiObjectName(GetGuiTrace(editor.gui.object)) ==
			GetGuiObjectName(widPhysics.shapeTxl) then

			local wheelDiff = -(GetMouseWheel() - widPhysics.prevMouseWheel)
			if wheelDiff ~= 0 then
				offset = GetTextListOffset(widPhysics.shapeTxl)+wheelDiff
				if offset < 0 then offset = 0 end
				if offset > diff then offset = diff end

				SetTextListOffset(widPhysics.shapeTxl, offset)
				SetSliderValue(widPhysics.shapeScrollBar, 1.0-(offset/diff))
			end
		end
	else
		SetSliderValue(widPhysics.shapeScrollBar, 1.0)
	end

	widPhysics.prevMouseWheel = GetMouseWheel()
end

