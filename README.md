McOrder
===========

A Little tool to find how to satisfy total-180 limit of breakfast of Synology.

## Requirement 
For practice C++11, some of this program written by C++11
You need to have environment to build C++11 project.

If you install latest Mingw, build.bat will work.

## Usage

Two modes: Group, and Calculation

### Group mode
interactive mode to set group constraint
	each group, you can set the minimum and maximum items in single order.
	for example, you don't want some items, create a group with maximum item to 0.
	or you want some items to appear in your order, create a group with minimum item with same amount of the group size.

### Calculation mode
In this mode, find all orders with the group constraint.

for more detail, see McOrder -h
