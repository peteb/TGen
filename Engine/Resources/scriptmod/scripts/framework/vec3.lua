--
--  vec3.lua
--  TGen Engine
--
--  Created by Peter Backman on 2/12/09.
--  Copyright 2009 Peter Backman. All rights reserved.
--


---------------------------------------------------------------------------------------
-- A vector with 3 components that describe a direction or point in 3D.
-- @class table
-- @name vec3
-- @field x Value along X axis
-- @field y Value along Y axis
-- @field z Value along Z axis

vec3 = {};
vec3.__index = vec3;


---------------------------------------------------------------------------------------
-- Default constructor for vec3.
-- @param vx The value along the X axis
-- @param vy The value along the Y axis
-- @param vz The value along the Z axis
-- @return A new 3-component vector (vec3)
function vec3.new(vx, vy, vz)
	local ret = {x = vx, y = vy, z = vz}
	setmetatable(ret, vec3);
	
	return ret;
end


---------------------------------------------------------------------------------------
-- Constructor for vec3, normalizes input.
-- @param vx The value along the X axis
-- @param vy The value along the Y axis
-- @param vz The value along the Z axis
-- @return A new normalized 3-component vector (vec3)
function vec3.newNormalize(vx, vy, vz)
	local newd = vec3.new(vx, vy, vz);
	newd:normalize();
	
	return newd;
end


---------------------------------------------------------------------------------------
-- Constructor, Zero vector.
-- @return A zero-vector
function vec3.zero()
	return vec3.new(0, 0, 0);
end


---------------------------------------------------------------------------------------
-- Constructor, clones the vector.
-- @return A new copy of the vector.
function vec3:clone() 
	return vec3.new(self.x, self.y, self.z);
end


---------------------------------------------------------------------------------------
-- Metamethod for tostring operator.
-- @return The vector formatted as a string, (x, y, z)
function vec3:__tostring()
	return "(" .. self.x .. ", " .. self.y .. ", " .. self.z .. ")";
end


---------------------------------------------------------------------------------------
-- Metamethod for + operator.
-- @param other The other vector to add
-- @return A new vec3 that is the sum of the two vectors
function vec3:__add(other) 
	return vec3.new(self.x + other.x, self.y + other.y, self.z + other.z);
end


---------------------------------------------------------------------------------------
-- Metamethod for - operator.
-- @param other The vector to subtract with
-- @return A new vec3 that is the difference between the two vectors
function vec3:__sub(other) 
	return vec3.new(self.x - other.x, self.y - other.y, self.z - other.z);
end


---------------------------------------------------------------------------------------
-- Metamethod for * operator.
-- @param scalar The value to multiply the vector with
-- @return A new vec3 that has been scaled with the value
function vec3:__mul(scalar)
	return vec3.new(self.x * scalar, self.y * scalar, self.z * scalar);
end


---------------------------------------------------------------------------------------
-- Metamethod for negotiation (-) operator.
-- @return The negated vector
function vec3:__unm()
	return vec3.new(-self.x, -self.y, -self.z);
end


---------------------------------------------------------------------------------------
-- Metamethod for .. operator.
function vec3:__concat(before)
	return before .. tostring(self);
end


---------------------------------------------------------------------------------------
-- Calculates length of vector.
-- @return The length of the vector
function vec3:magnitude()
	return math.sqrt(self.x * self.x + self.y * self.y + self.z * self.z)
end


---------------------------------------------------------------------------------------
-- Calculates the unit vector with the same direction.
-- If the length is 0, nil will be returned.
-- @return A new vec3 that is the normalized version of this vector
function vec3:normalize()
	local length = self:magnitude();
	
	if (length == 0.0) then
		return nil;
	end
		
	self.x = self.x / length;
	self.y = self.y / length;
	self.z = self.z / length;
	
	return self;
end


---------------------------------------------------------------------------------------
-- Computes the dot product of two vectors.
-- The parameters are commutative.
-- @param v1 The first vector
-- @param v2 The second vector
-- @usage <code>local dp = vec3.dotProduct(v1, v2)</code>
-- @usage <code>local dp = v1:dotProduct(v2)</code>
function vec3.dotProduct(v1, v2)
	return vec3.new(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
end


---------------------------------------------------------------------------------------
-- Computes the cross product vector of two vectors.
-- The parameters are not commutative.
-- @param v1 The first vector
-- @param v2 The second vector
-- @usage <code>local normal = vec3.crossProduct(v1, v2)</code>
-- @usage <code>local normal = v1:crossProduct(v2)</code>
function vec3.crossProduct(v1, v2)
	return vec3.new(v1.x * v2.z - v2.y * v1.z, v1.z * v2.x - v2.z * v1.x, v1.x * v2.y - v2.x * v1.y);
end


