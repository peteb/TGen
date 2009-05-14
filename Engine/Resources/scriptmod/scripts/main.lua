

--function entities.box1:onCreation() 
--	print("BOX CREATED with name "..self:name());
--end

import "sound/musicplayer.lua"

for k,v in pairs(entities) do
	print(k.." with name: "..v:name());      
	
	-- register onCreation
	v.onCreation = function(self)
		--print("*** Entity created: " .. self:name());
		local message = "";

		for k,v in pairs(v) do
			if (string.sub(tostring(k), 0, 1) ~= "_") then
				message = message.."   "..type(v).." "..tostring(k).."\n";
			end
		end
		
		--env.showMessage("Created entity \""..self:name().."\"", message);
	end
	
	--print("   components:");
	
end


local funkyNode = entities.box1.sceneNode;
local worldPos = funkyNode:worldPosition();


print("World position: " .. worldPos);
print("Local position: " .. [funkyNode localPosition]);

print("Added: " .. worldPos + vec3.new(0.0, 2.0, 1.0));


local newMaterial = [resources material: "newmat"];

print("New material's name: "..[newMaterial name]);
funkyNode:setMaterial(newMaterial);

function entities.player.physGeom:onCollision(force, with)
	--print("player collision, force: "..force.." with: "..with:owner():name());
	--env.showMessage("Stuff", "hej");
	--env.quit();
	--print("Player speed: "..self:link():worldVelocity():magnitude());
end


entities.box1.teleinSound = [resources sound:"telein.wav"];

function entities.teleportground.physGeom:onCollision(force, with)
	if ([with owner].worldInterface) then
		print("TELEPORT ACTIVATED on "..[[with owner] name]);
		
		if ([with link].name) then
			print("Name of with's body: "..[[with link] name]);
		end 
		
		[nil setStuff:[nil getStuff:[nil doFlough]]];	-- this is ok
		
		print("Link position: "..tostring([[with link] worldPosition]));
		print("Owner worldpos: " .. tostring([[with owner].worldInterface worldPosition]));
		
		[[with link] resetForces];
		[[with owner].worldInterface setWorldPosition: [entities.teleportstart.worldInterface worldPosition]];
		[entities.teleportstart playTeleportSound];
		
		[nil executeThing:"gross" withR: 12.34 andG:"bo"];
		
	end
end



entities.teleportstart.teleportSound = [resources sound:"telein.wav"];

function entities.teleportstart:playTeleportSound()
	[self.soundLocal playSound: self.teleportSound];
end

