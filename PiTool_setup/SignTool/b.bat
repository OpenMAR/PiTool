signtool sign /v /fd sha1 /ac MSCV-GSClass3.cer /sha1 5d0edc21df734eb0fd34a39c0d1fdecd7d849bb2 /tr http://rfc3161timestamp.globalsign.com/advanced /td sha256 all_files/*.* 
Pause
signtool sign /v /fd sha256 /ac MSCV-GSClass3.cer /as /sha1 5d0edc21df734eb0fd34a39c0d1fdecd7d849bb2 /tr http://rfc3161timestamp.globalsign.com/advanced /td sha256 all_files/*.* 
Pause

