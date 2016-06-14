import pip
import requests

la = [-8, -8.000001, -8.000002, -8.000003, -8.000004, -8.000005, -8.000006]
lg = [-35, -35.000000, -35.000002, -35.000003, -35.000004, -35.000005, -35.000006]
for i in range (0, 7):
    dados = {'latitude':la[i],'longitude':lg[i],'classificacao':'RE'}
    r = requests.post("http://localhost:8080/php/receiver_v1.php", data=dados) 
    print ("well done!\n", r,'\n', dados)
