

--function entities.box1:onCreation() 
--	print("BOX CREATED with name "..self:name());
--end

import "sound/musicplayer.lua"
make_musicplayer(entities.music);

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



-- TODO: fixa onCollision bättre, så den skickar med kollisioninfo
--		som en table i parameter

function entities.player.physGeom:onCollision(force, with)
	--print("player collision, force: "..force.." with: "..with:owner():name());
	--env.showMessage("Stuff", "hej");
	--env.quit();
	--print("Player speed: "..self:link():worldVelocity():magnitude());
end

-- TODO: onFirstCollision ska se ut exakt som onCollision, dvs ha med with och info

function entities.player.physGeom:onFirstCollision(with, info)		-- Inte bra att ha för squeeze...
	--info = {
	--	force,	-- gör force sen, ta med grundläggande saker först
	--	axis,
	--}
	print("First step");
end


propertyTable = {
	__newindex = function(self, key, value)
		local rawkey = rawget(self, "_"..key);
		
		if (rawkey) then
			if (rawkey.constraint) then
				if (rawkey.constraint(value)) then
					rawkey.value = value;
				else
					error("Constraint failed: "..rawkey.constraintString);
				end
			else
				rawkey.value = value;
			end
		end
	end;
	
	__index = function(self, key)
		return rawget(self, "_"..key).value;
	end;
}


local code = filesystem:openRead("/classes"):all();
local parsed = parse_properties(code);

function createClass(name, contents)
	local newClass = {}
	
	for k, v in pairs(contents) do		-- loop attributes
		local newAttribute = {};
		newAttribute.value = v.default;

		local code = "return function(value) return " .. v.constraint .. "; end;";
		local con, error = loadstring(code)();

		if (con == nil) then
			error("Failed to compile constraint: " .. error);
		end
		
		newAttribute.constraint = con;
		newAttribute.constraintString = v.constraint;
		newClass["_"..k] = newAttribute;
	end

	setmetatable(newClass, propertyTable);
	
	return newClass;
end


for k, v in pairs(parsed) do
	local newClass = createClass(k, v);
	_G[k] = newClass;
	--env.showMessage("Created class "..k..":\n"..printProperties(newClass));
end

--Person.age = -3;
--env.showMessage(Person.age);


--env.quit();

