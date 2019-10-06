使用程式前需先使用指令make生成 405235035_hw5

輸入./405235035_hw5 即可執行程式 
此程式執行時會自動讀取當前所在位置下的input.txt檔案
input.txt的第一行的內容分別為hash table的行數、列數及給定的質數
之後會讀取input.txt中的內容，將每個英文單詞切斷 (若單詞結尾有.或,則會消除)
並將之轉為小寫後利用hash function找出hash value，然後根據此值改寫hash table中適當的位置的值

全部讀取完畢後會進入query mode
可以輸入一個英文單詞來查看該單詞出現的次數(有誤差)
若輸入exit則會退出程式，並釋放記憶體

input.txt中每行字串最大長度可用 MAX_STRING 修改，預設為10240

額外功能：
1.此程式利用輸入exit結束時會將所malloc出的記憶體全數釋放

執行環境為csie0.cs.ccu.edu.tw

405235035 資工三 王博輝 b121417393@gmail.com
