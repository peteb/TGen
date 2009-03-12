

--function entities.box1:onCreation() 
--	print("BOX CREATED with name "..self:name());
--end

for k,v in pairs(entities) do
	print(k.." with name: "..v:name());      
	
	-- register onCreation
	v.onCreation = function(self)
		print("*** Entity created: " .. self:name());
	end
	
	--print("   components:");
	
	--for k,v in pairs(v) do
	--	if (type(v) == "table" and v.worldPosition) then
	--		print("         worldpos: " .. v:worldPosition());
	--	end
	--end
end


local funkyNode = entities.box1.sceneNode;
local worldPos = funkyNode:worldPosition();

local newMaterial = resources:material("newmat");

print("World position: " .. worldPos);
print("Local position: " .. funkyNode:localPosition());

print("Added: " .. worldPos + vec3.new(0.0, 2.0, 1.0));

print("New material's name: "..newMaterial:name());

funkyNode:setMaterial(newMaterial);

