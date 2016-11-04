--- ��ƿ� �������̽� ��� �ڵ� �����ϴ� ��ũ��Ʈ
--- �Է����� : zz_interface.h
--- ������� : zz_api_define.inc zz_api_register.inc
---
--- ss checkout $/znzin11/engine/include/zz_api_define.inc $/znzin11/engine/include/zz_api_register.inc -yzho,XXXX -i-y
---
--- $Header: /Client/export_GM.lua 2     03-08-29 5:22p Icarus $

--- ������ �о� interfaces ���̺� ���� ����
function read_interfaces ( hin, hout )
    i = 1
    word = read( hin, "*w" )

    while ( word ) do
        if (word ~= "GF_SCRIPT") then
            word = read( hin, "*l" ) -- skip current line
        else
            local return_type
            local name
            return_type = read( hin, "*w" )
            if (not return_type) then break end

            name = read( hin, "*w" )
            if (not name) then break end

            interfaces[name] = {}

            interfaces[name].return_type = return_type

            word = read( hin, "*w" ) -- skip (
            if (not word) then break end

            word = read( hin, "*w" )
            if (not word) then break end

            interfaces[name].params = {}
            local param_index = 1
            
            while (word ~= ");") do
                -- extract out comma(,)
                interfaces[name].params[param_index] = gsub( word, ",", "" )
                word = read( hin, "*w" ) -- skip parameter name
                if (not word) then break end
                if (word == ");") then break end
                word = read( hin, "*w" ) -- read next
                if (not word) then break end
                param_index = param_index + 1
            end
            i = i + 1
        end -- end of if
        word = read( hin, "*w" ) -- read next
    end -- end of while
end

IN_FILENAME   = "Game_FUNC.h"
OUT_FILENAME1 = "Game_FUNC_DEF.inc"
OUT_FILENAME2 = "Game_FUNC_REG.inc"

hin   = readfrom ( IN_FILENAME   ) -- input file handle
hout1 = writeto  ( OUT_FILENAME1 ) -- output file handle
hout2 = writeto  ( OUT_FILENAME2 ) -- output file handle

interfaces = {} -- �������̽� ���� ���̺�

--- �������̽� ���� �о���̱�
read_interfaces ( hin, hout )

names = {} --- ���Ŀ� �̸� ���̺�
i = 1
index = next(interfaces, nil) -- �̸����� �����ϴ� ���̺��̹Ƿ�, interfaces[i] �� ����� �� ����.
while (index) do
    names[i] = index
    index = next(interfaces, index)
    i = i + 1
end

--- �̸������� �����ϱ�
sort( names )

--- ��ũ�� ���� ���̺��
map_return_types = {
    void    = "RETURNS_NONE",
    int     = "RETURNS_INT",
    float   = "RETURNS_FLOAT",
    HNODE   = "RETURNS_HNODE",
    ZSTRING = "RETURNS_STRING"
}
map_param_types = {
    void    = "",
    int     = "ZL_INT",
    float   = "ZL_FLOAT",
    HNODE   = "ZL_HNODE",
    ZSTRING = "ZL_STRING"
}

--- ��� �޼���
message_autogen =
"//==========================================================================//\n"..
"// �� ������ export_interface.lua ��ũ��Ʈ�� ���� �ڵ������� �ڵ��Դϴ�.\n"..
"// ���� �������� ���ʽÿ�.\n"..
"// "..date().."\n"..
"//==========================================================================//\n\n\n"

write( hout1, message_autogen )
write( hout2, message_autogen )

--- ��ũ��Ʈ �������̽� ���ǿ� ���� �����ϱ�
for i = 1, getn(names) do
    definition = format( "ZL_FUNC_BEGIN( %s, ", names[i] )
    -- export return_type
    definition = definition..format( "%s", map_return_types[ interfaces[names[i]].return_type ] )
    definition = definition.." )\n"
    
    for j = 1, getn(interfaces[names[i]].params) do
        if (j == getn(interfaces[names[i]].params)) then
            definition = definition..format( "\t%15s\n", map_param_types[ interfaces[names[i]].params[j] ] )
        else
            definition = definition..format( "\t%15s,\n", map_param_types[ interfaces[names[i]].params[j] ] )
        end
    end

    definition = definition.."ZL_FUNC_END\n\n"
        
    write( hout1, definition )
end

--- ��ũ��Ʈ �������̽� ��Ͽ� ���� �����ϱ�
for i = 1, getn(names) do
    register = format( "ZL_REGISTER( %-30s )\n", names[i] )
    write( hout2, register )
end

--- ������ �� ǥ��
message_end_of_file = "// end of file\n"

write( hout1, message_end_of_file )
write( hout2, message_end_of_file )

closefile( hin   )
closefile( hout1 )
closefile( hout2 )

