--
--  vector.lua
--  TGen Engine
--
--  Created by Peter Backman on 2/12/09.
--  Copyright 2009 Peter Backman. All rights reserved.
--
 
vec3 = {}
vec3.__index = vec3;

function vec3.new(vx, vy, vz)
	local ret = {x = vx, y = vy, z = vz}
	setmetatable(ret, vec3);
	
	return ret;
end

function vec3.newNormalize(vx, vy, vz)
	local newd = vec3.new(vx, vy, vz);
	newd:normalize();
	
	return newd;
end

function vec3:__tostring()
	return "(" .. self.x .. ", " .. self.y .. ", " .. self.z .. ")";
end

function vec3:__add(other) 
	return vec3.new(self.x + other.x, self.y + other.y, self.z + other.z);
end

function vec3:__sub(other) 
	return vec3.new(self.x - other.x, self.y - other.y, self.z - other.z);
end

function vec3:__mul(scalar)
	return vec3.new(self.x * scalar, self.y * scalar, self.z * scalar);
end

function vec3:__unm()
	return vec3.new(-self.x, -self.y, -self.z);
end

function vec3:__concat(before)
	return before .. tostring(self);
end

function vec3:magnitude()
	return math.sqrt(self.x * self.x + self.y * self.y + self.z * self.z)
end

function vec3:normalize()
	local length = self:magnitude();
	
	if (length == 0.0) then
		return;
	end
		
	self.x = self.x / length;
	self.y = self.y / length;
	self.z = self.z / length;
	
	return self;
end

function vec3.dotProduct(v1, v2)
	return vec3.new(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
end

function vec3.crossProduct(v1, v2)
	return vec3.new(v1.x * v2.z - v2.y * v1.z, v1.z * v2.x - v2.z * v1.x, v1.x * v2.y - v2.x * v1.y);
end

vec3.Zero = vec3.new(0.0, 0.0, 0.0);

