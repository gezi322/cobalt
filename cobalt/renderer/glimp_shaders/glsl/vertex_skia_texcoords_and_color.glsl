#version 100
uniform mat3 uViewM;
uniform vec4 urtAdjustment;
attribute vec2 inPosition;
attribute vec4 inColor;
attribute vec2 inTextureCoords;
varying vec4 vColor;
varying vec2 vtextureCoords_Stage0;
void main()
{
  vec3 pos3 = uViewM * vec3(inPosition, 1);
  vColor = inColor;
  {
    // Stage 0: Texture
    vtextureCoords_Stage0 = inTextureCoords;
  }
  gl_Position = vec4(dot(pos3.xz, urtAdjustment.xy),
                     dot(pos3.yz, urtAdjustment.zw), 0, pos3.z);
}

