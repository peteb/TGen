--
--  mat3.lua
--  TGen Engine
--
--  Created by Peter Backman on 3/18/09.
--  Copyright 2009 Peter Backman. All rights reserved.
--
 
mat3 = {}
mat3.__index = mat3;

function mat3.new(xaxis, yaxis, zaxis)
	local ret = {x = xaxis, y = yaxis, z = zaxis}
	setmetatable(ret, mat3);
	
	return ret;
end

function mat3.identity()
	return mat3.new(vec3.new(1.0, 0.0, 0.0), vec3.new(0.0, 1.0, 0.0), vec3.new(0.0, 0.0, 1.0));
end

function mat3.rotationX(angle)
	local x = vec3.new(1.0, 0.0, 0.0);
	local y = vec3.new(0.0, math.cos(angle), math.sin(angle));
	local z = vec3.new(0.0, -math.sin(angle), math.cos(angle));
	
	return mat3.new(x, y, z);
end

function mat3.rotationY(angle)
	local x = vec3.new(math.cos(angle), 0.0, -math.sin(angle));
	local y = vec3.new(0.0, 1.0, 0.0);
	local z = vec3.new(math.sin(angle), 0.0, math.cos(angle));

	return mat3.new(x, y, z);
end

function mat3.rotationZ(angle)
	local x = vec3.new(math.cos(angle), math.sin(angle), 0.0);
	local y = vec3.new(-math.sin(angle), math.cos(angle), 0.0);
	local z = vec3.new(0.0, 0.0, 1.0);
	
	return mat3.new(x, y, z);
end

function mat3.rotation(axis, angle)
	local c = math.cos(angle);
	local s = math.sin(angle);
	local omc = 1.0 - c;
	
	local x = vec3.new(1.0 + omc * (axis.x * axis.x - 1.0), 
					   axis.z * s + omc * axis.x * axis.y, 
					   -axis.y * s + omc * axis.x * axis.z);
	local y = vec3.new(-axis.z * s + omc * axis.x * axis.y,
					   1.0 + omc * (axis.y * axis.y - 1.0),
					   axis.x * s + omc * axis.y * axis.z);
	local z = vec3.new(axis.y * s + omc * axis.x * axis.z,
					   -axis.x * s + omc * axis.y * axis.z,
					   1.0 + omc * (axis.z * axis.z - 1.0));

	return mat3.new(x, y, z);
end

function mat3.lookInDirection(dir, up)
	local look = dir;
	local right = vec3.crossProduct(up, look);
	
	if (right:magnitude() < 0.5) then
		right = vec3.crossProduct(vec3.new(0, 0, 1), look);
	end
	
	local newUp = vec3.crossProduct(look, right);
	
	return mat3.new(right, newUp, look);
end

function mat3:__mul(other)
	local ret = mat3.new(vec3.new(0, 0, 0), vec3.new(0, 0, 0), vec3.new(0, 0, 0));
	
	local iterOther = other:iterationMap();
	local iterSelf = self:iterationMap();
	local iterRet = ret:iterationMap();
		
	for i = 1, 3 do
		for j = 1, 3 do
			iterRet[i][j] = 0;

			for k = 1, 3 do
				iterRet[i][j] = iterRet[i][j] + iterSelf[i][k] * iterOther[k][j];
			end
		end
	end
		
	return mat3.newFromIterationMap(iterRet);
end

function mat3:__concat(before)
	return before .. tostring(self);
end

function mat3:__tostring()
	local row1 = self.x.x .. ", " .. self.y.x .. ", " .. self.z.x;
	local row2 = self.x.y .. ", " .. self.y.y .. ", " .. self.z.y;
	local row3 = self.x.z .. ", " .. self.y.z .. ", " .. self.z.z;
	
	return row1 .. "\n" .. row2 .. "\n" .. row3;
end

function mat3:iterationMap()
	local ret = {};
	
	ret[1] = {}
	ret[2] = {}
	ret[3] = {}

	ret[1][1] = self.x.x;
	ret[1][2] = self.x.y;
	ret[1][3] = self.x.z;

	ret[2][1] = self.y.x;
	ret[2][2] = self.y.y;
	ret[2][3] = self.y.z;

	ret[3][1] = self.z.x;
	ret[3][2] = self.z.y;
	ret[3][3] = self.z.z;
	
	return ret;
end

function mat3.newFromIterationMap(map)
	local ret = {x = vec3.new(map[1][1], map[1][2], map[1][3]), y = vec3.new(map[2][1], map[2][2], map[2][3]), z = vec3.new(map[3][1], map[3][2], map[3][3])}
	setmetatable(ret, mat3);
	
	return ret;
end
