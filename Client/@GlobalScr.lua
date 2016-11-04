-- ���� �̸����� ����Ʈ�� ��´�.
-- int	GF_GetEffectUseFile ( ZSTRING szEffectFile )

-- LIST_EFFECT.stb �� index�� ����Ʈ�� ��´�.
-- int	GF_GetEffectUseIndex ( int iEffectIndex )

-- iObjec����  effect�� ǥ���Ѵ�.
-- void GF_EffectOnObject ( int iObject, int iEffectHash )

-- �ɸ��� ������ ȣ��Ǵ°�
function ON_CHAR_APPEAR ( iObject )
	iEffect = GF_GetEffectUseFile( "3DDATA/Effect/apear_01.eft" )
	GF_EffectOnObject( iObject, iEffect );
end

-- �������� ȣ��Ǵ°�
function ON_CHAR_LEVEL_UP ( iObject )
	iEffect = GF_GetEffectUseFile( "3DDATA/Effect/levelup_01.eft" )
	GF_EffectOnObject( iObject, iEffect );
end
	
-- �Ҹ� ������ ����Ҷ�...
function ON_USE_ITEM ( iObject )
	iEffect = GF_GetEffectUseFile( "3DDATA/Effect/recover_01.eft" )
	GF_EffectOnObject( iObject, iEffect );
end


-- ��ų �����ϴ°�( 1~5 )...
function DO_SKILL ( iObject, iSkillIDX )
	iEffect = GF_GetEffectUseFile( "3DDATA/Effect/apear_01.eft" )

	iTarget = GF_GetTarget( iObject );

	GF_EffectOnObject( iTarget, iEffect );
	GF_EffectOnObject( iObject, iEffect );

		iMotion = GF_GetMotionUseFile( "3DDATA/MOTION/AVATAR/empty_jump2_m1.ZMO" )

	GF_SetMotion( iObject, iMotion );
end


