entities.box1.materialChanger.materials = {
	resources:material("newmat"),
	resources:material("greenmaterial"),
	resources:material("coolDoom3Mat")
}


entities.box1.materialChanger.lastMaterial = 0;

local box = entities.box1;

function box.materialChanger:onTick()
	self.lastMaterial = (self.lastMaterial + 1) % #self.materials;

	box.sceneNode:setMaterial(self.materials[self.lastMaterial + 1]);

	self:setInterval(self:interval() + 0.1);

	--print(self:name().." interval: "..self:interval());
	
	if (self:interval() > 1.5) then
		self:disable();
	elseif (self:interval() > 1.2) then
		box.timer2:enable();
		entities.box1.soundLocal:playSound(entities.box1.teleinSound);

	end
	
end


function entities.box1.soundLocal:onChannelFinished() 
	print(">> CHANNEL FINISHED");
end

box.timer2.numTicks = 0;

function box.timer2:onTick() 
	self.numTicks = self.numTicks + 1;

	--print(self:name().." tick: "..self.numTicks);
	
	oldPos = box.sceneNode:localPosition();
	
	local pos = self.numTicks;
	if (pos >= 5.0) then
		pos = 5.0 - (pos - 5.0);
	end
	
	local myVec = vec3.new(1, 2, 3);
	myVec:normalize();
	
	--box.sceneNode:setLocalPosition(vec3.new(0.0, pos * 0.3, oldPos.z));
	local myMat = mat3.rotation(box.sceneNode:localOrientation().x, math.rad(10));
	

		
	--env.quit();

	[box.sceneNode multiplyLocalOrientation: myMat];
	[box.physBody enable];
	
	--box.sceneNode:multiplyLocalOrientation(myMat);	
	--box.physBody:enable();
	
	--box.sceneNode:setLocalOrientation(box.sceneNode:localOrientation() * myMat);
	
	--box.sceneNode:setLocalOrientation(box.sceneNode:localOrientation() * myMat);
	-- TODO: multiplyLocalOrientation
	
	if (self.numTicks >= 10) then
		self.numTicks = 0;
		[self disable];
		[box.materialChanger setInterval: 1.0];
		[box.materialChanger enable];
	end
end

-- Interpolator = Position/Orientation, tar ett objekt som Šr interpolator function typ wave 
-- fšrst; scripta med phys*, sen ljud


