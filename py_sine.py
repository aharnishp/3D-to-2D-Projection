import math

output = []
for deg in range(0,91):
    output.append(int(math.cos(deg*math.pi/180)*255))

print(output)
print(len(output))