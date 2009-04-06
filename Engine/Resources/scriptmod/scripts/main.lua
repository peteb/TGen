

--function entities.box1:onCreation() 
--	print("BOX CREATED with name "..self:name());
--end

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
print("Local position: " .. funkyNode:localPosition());

print("Added: " .. worldPos + vec3.new(0.0, 2.0, 1.0));


local newMaterial = resources:material("newmat");

print("New material's name: "..newMaterial:name());
funkyNode:setMaterial(newMaterial);

function entities.player.physGeom:onCollision(force, with)
	--print("player collision, force: "..force.." with: "..with:owner():name());
	--env.showMessage("Stuff", "hej");
	--env.quit();
	print("Player speed: "..self:link():worldVelocity():magnitude());
end


entities.box1.teleinSound = resources:sound("telein.wav");

entities.player.worldInterface = entities.player.origin;
entities.box1.worldInterface = entities.box1.sceneNode;


function entities.teleportground.physGeom:onCollision(force, with)
	if (with:owner().worldInterface) then	-- TODO: worldInterface in entity def, read only. entity has a worldInterface if any positional component exists
		print("TELEPORT ACTIVATED on "..with:owner():name());
		
		if (with:link().name) then
			print("Name of with's body: "..with:link():name());
		end
			
		-- TODO: link(), return the LINK!!!!
		--		if there is no body, construct a new table that wraps WorldObject and return it from link()
		
		if (with:link().resetForces) then
			print("Link implements resetForces!");
			with:link():resetForces();
		end
		
		if (with:link().worldPosition) then
			print("Link implements worldPosition: "..with:link():worldPosition());
		else
			print("NO WORLDPOSITION");
		end
		
		with:owner().worldInterface:setWorldPosition(entities.teleportstart.sceneNode:worldPosition());
		entities.teleportstart:playTeleportSound();
	end
end



entities.teleportstart.teleportSound = resources:sound("telein.wav");

function entities.teleportstart:playTeleportSound()
	self.soundLocal:playSound(self.teleportSound);
end

