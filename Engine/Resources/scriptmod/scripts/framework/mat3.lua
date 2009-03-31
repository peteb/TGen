--
--  mat3.lua
--  TGen Engine
--
--  Created by Peter Backman on 3/18/09.
--  Copyright 2009 Peter Backman. All rights reserved.
--
 
 
---------------------------------------------------------------------------------------
-- A 3x3 matrix, most commonly used for rotations.
-- @class table
-- @name mat3
-- @field x The X axis.
-- @field y The Y axis.
-- @field z The Z axis.

mat3 = {}
mat3.__index = mat3;


---------------------------------------------------------------------------------------
-- Default constructor for mat3.
-- @param xaxis The X axis.
-- @param yaxis The Y axis.
-- @param zaxis The Z axis.
-- @return A new 3x3 matrix.
function mat3.new(xaxis, yaxis, zaxis)
	local ret = {x = xaxis, y = yaxis, z = zaxis}
	setmetatable(ret, mat3);
	
	return ret;
end


---------------------------------------------------------------------------------------
-- Identity constructor.
-- @return A new identity matrix
function mat3.identity()
	return mat3.new(vec3.new(1.0, 0.0, 0.0), vec3.new(0.0, 1.0, 0.0), vec3.new(0.0, 0.0, 1.0));
end


---------------------------------------------------------------------------------------
-- Rotation around X axis.
-- @param angle Angle around X in radians.
-- @return A new 3x3 matrix that describes the rotation
function mat3.rotationX(angle)
	local x = vec3.new(1.0, 0.0, 0.0);
	local y = vec3.new(0.0, math.cos(angle), math.sin(angle));
	local z = vec3.new(0.0, -math.sin(angle), math.cos(angle));
	
	return mat3.new(x, y, z);
end


---------------------------------------------------------------------------------------
-- Rotation around Y axis.
-- @param angle Angle around y in radians.
-- @return A new 3x3 matrix that describes the rotation
function mat3.rotationY(angle)
	local x = vec3.new(math.cos(angle), 0.0, -math.sin(angle));
	local y = vec3.new(0.0, 1.0, 0.0);
	local z = vec3.new(math.sin(angle), 0.0, math.cos(angle));

	return mat3.new(x, y, z);
end


---------------------------------------------------------------------------------------
-- Rotation around Z axis.
-- @param angle Angle around Z in radians.
-- @return A new 3x3 matrix that describes the rotation
function mat3.rotationZ(angle)
	local x = vec3.new(math.cos(angle), math.sin(angle), 0.0);
	local y = vec3.new(-math.sin(angle), math.cos(angle), 0.0);
	local z = vec3.new(0.0, 0.0, 1.0);
	
	return mat3.new(x, y, z);
end


---------------------------------------------------------------------------------------
-- Rotation around an arbitrary axis.
-- @param axis The 3D axis to rotate around
-- @param angle Angle around axis in radians.
-- @return A new 3x3 matrix that describes the rotation
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


---------------------------------------------------------------------------------------
-- Rotation from direction and up-vectors.
-- @param dir The axis that the Z axis should be aligned to.
-- @param up The up axis.
-- @return A new 3x3 matrix that describes the rotation.
function mat3.lookInDirection(dir, up)
	local look = dir;
	local right = vec3.crossProduct(up, look);
	
	if (right:magnitude() < 0.5) then
		right = vec3.crossProduct(vec3.new(0, 0, 1), look);
	end
	
	local newUp = vec3.crossProduct(look, right);
	
	return mat3.new(right, newUp, look);
end


---------------------------------------------------------------------------------------
-- Metamethod for * operator.
-- * operations on mat3 are not commutative.
-- @param other The other 3x3 matrix to multiply with.
-- @return A new 3x3 matrix that is the product of the two matrices.
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


---------------------------------------------------------------------------------------
-- Metamethod for '..' operator.
function mat3:__concat(before)
	return before .. tostring(self);
end


---------------------------------------------------------------------------------------
-- Metamethod for tostring operator.
-- @return The matrix formatted as a string, columns being vectors.
function mat3:__tostring()
	local row1 = self.x.x .. ", " .. self.y.x .. ", " .. self.z.x;
	local row2 = self.x.y .. ", " .. self.y.y .. ", " .. self.z.y;
	local row3 = self.x.z .. ", " .. self.y.z .. ", " .. self.z.z;
	
	return row1 .. "\n" .. row2 .. "\n" .. row3;
end


---------------------------------------------------------------------------------------
-- An iteration map that can be used for iterating the components of the matrix.
-- @return A multidimensional array that points to the components of the matrix.
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


---------------------------------------------------------------------------------------
-- Construct 3x3 matrix from iteration map.
-- @param map The iteration map.
-- @return A new 3x3 matrix.
function mat3.newFromIterationMap(map)
	local ret = {x = vec3.new(map[1][1], map[1][2], map[1][3]), y = vec3.new(map[2][1], map[2][2], map[2][3]), z = vec3.new(map[3][1], map[3][2], map[3][3])}
	setmetatable(ret, mat3);
	
	return ret;
end
