entities.box1.materialChanger.materials = {
	resources:material("newmat"),
	resources:material("greenmat"),
	resources:material("coolDoom3Mat")
}


entities.box1.materialChanger.lastMaterial = 0;

local box = entities.box1;

function box.materialChanger:onTick()
	-- TOOD: timer:interval, setInterval, enable, disable
	self.lastMaterial = (self.lastMaterial + 1) % #self.materials;

	box.sceneNode:setMaterial(self.materials[self.lastMaterial + 1]);
end
