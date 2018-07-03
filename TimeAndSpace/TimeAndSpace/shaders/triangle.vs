//
//  triangle.vs
//  TimeAndSpace
//
//  Created by Carry on 2018/7/2.
//  Copyright © 2018 Carry. All rights reserved.
//

#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
