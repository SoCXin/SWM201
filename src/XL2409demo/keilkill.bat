del *.log /s
del *.bak /s
del *.ddk /s
del *.edk /s
del *.lst /s
del *.lnp /s
del *.mpf /s
del *.mpj /s
del *.obj /s
del *.omf /s
::del *.opt /s  ::������ɾ��JLINK������
del *.plg /s
del *.rpt /s
del *.tmp /s
del *.__i /s
del *.crf /s
del *.o /s
del *.d /s
del *.axf /s
del *.tra /s
del *.dep /s           
del JLinkLog.txt /s

del *.iex /s
del *.htm /s
del *.sct /s
del *.map /s

del *.hex /s
del *.bin /s
del *.fw  /s

del *.uvguix.* /s

del *.Administrator /s
del *.saved_uv4 /s

del *.c.orig /s
del *.h.orig /s

cd Project
rd /q /s "OBJ"
rd /q /s "RTE"

exit
