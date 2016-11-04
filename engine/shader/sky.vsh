vs.1.1
//---------------------------------------------------------------
// c0  = world-view matrix
// c4  = projection matrix
// c8  = light texture tm
// c12 = light diffuse
// c13 = light ambient
// c14 = light direction (or position)
// c15 = camera position (world)
// c16 = [1.0, .5, 0.0, 127.9961]
// c17 = C_ALPHA_FOG_START_END_FACTOR = [fog_start, fog_end, 1/(fog_end - fog_start), 0]
// ...
// c30-95 = bone matrix (max 22( 3x22 = 66) bone's modelview + world matrix)
//---------------------------------------------------------------
dcl_position      V_POSITION
dcl_normal        V_NORMAL
dcl_texcoord0     V_TEXCOORD0

//-------------------------------------------------
// compute position (oPos)
//-------------------------------------------------
m4x3 r4, V_POSITION, C_WORLDVIEW_TM // apply worlview matrix
mov r4.w, C_ONE_HALF_ZERO_MAXPOWER.x
mov r6, r4                     // for fog
m4x4 oPos, r4, C_PROJECTION_TM // apply projection matrix

//------------------------------------------------------------------------------
// Texture coordinates
//------------------------------------------------------------------------------
// Copy tex coords
mov oT0.xy, V_TEXCOORD0
mov oT1.xy, V_TEXCOORD0