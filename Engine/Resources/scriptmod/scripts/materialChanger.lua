entities.box1.materialChanger.materials = {
	resources:material("newmat"),
	resources:material("greenmat"),
	resources:material("coolDoom3Mat")
}


entities.box1.materialChanger.lastMaterial = 0;

local box = entities.box1;

function box.materialChanger:onTick()
	self.lastMaterial = (self.lastMaterial + 1) % #self.materials;

	box.sceneNode:setMaterial(self.materials[self.lastMaterial + 1]);

	self:setInterval(self:interval() + 0.1);

	print(self:name().." interval: "..self:interval());
	
	if (self:interval() > 1.5) then
		self:disable();
		box.timer2:enable();
	end
end

box.timer2.numTicks = 0;

function box.timer2:onTick() 
	self.numTicks = self.numTicks + 1;

	print(self:name().." tick: "..self.numTicks);
	
	if (self.numTicks >= 10) then
		self.numTicks = 0;
		self:disable();
		box.materialChanger:setInterval(1.0);
		box.materialChanger:enable();
	end
end