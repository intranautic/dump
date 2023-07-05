# copy-on-write is an optimization technique used in a shit ton of places
# linux kernel most notably, but is also used within dynamically typed language's
# runtime environments as well
# the general concept with copy on write is that when a resource is copied to another:
x = 1
y = x
print("x @ %#lx" % id(x))
print("y @ %#lx" % id(y))
# as long as the initial value has not been mutated, both x and y can share the same
# reference to the same object in memory. however, at the point in which y actually
# mutates the object:
y += 1
# then it will perform the copy and assign that new object to y
print("x @ %#lx" % id(x))
print("y @ %#lx" % id(y))
# hence, it will copy-on-write
