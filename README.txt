open_train_station - creeaza gara prin alocarea memoriei pentru platforme si initializeaza puterea locomotivelor cu -1 pentru a diferentia o platforma fara tren de una 
in care exista un tren ( exista trenuri cu putere 0)

close_train_station - elibereaza mai intai vagoanele, pe urma platformele si in final toata gara.

show_existing_trains - /

arrive_train - existenta trenului deja pe platforma este data de puterea acestuia, daca nu exista tren pe peron, se va atribui puterea locomotivei.

leave_train - elibereaza memoria in cazul vagoanelor si puterea locomotivei devine -1 pentru a arata ca nu exista un tren.

add_train_car - se aloca memorie vagonului si se adauga in lista de tip TrainCar regasita in structura lui platforms[platform]->train_cars. Vagonul se adauga la final.
Daca nu exista platforma, nu va face nimic.

remove_train_cars - atunci cand se elimina vagoanele, se elibereaza memoria. Folosesc variabila prev pentru a putea face legatura cu celalalte elemente de dupa elementul de sters.

move_train_cars - am evitat cazurile limita precum existenta statiei, platformelor si pozitiile sa fie > 0 si daca exista un tren pe platforme. 
Caut vagoanele de pe pozitiile cerute (start_car1, start_car2) si retin de asemenea elementele de dinainte prin intermediul variabilelor before_start1, before_start2.
Variabila goThrough reprezinta ultimul vagon din sirul de vagoane de mutat. Am comentat fiecare actiune pentru a fi mai clar ce se intampla in fiecare caz in parte.

find_express_train - caut trenul care are locomotiva - suma greutatii tuturor vagoanelor maxima.

find_overload_train - caut trenul ce are suma greutatii vagoanelor mai mare decat puterea locomotivei trenului.

find_optimal_train - este opusul find_express_train, locomotiva - suma greutatii tuturor vagoanelor sa fie minima si mai mare decat 0

find_heaviest_sequence_train - prin intermediul variabilei goThrough, trec de mai multe ori prin lista train_cars a fiecarui tren de pe platforme. 
Primul while este pentru a putea da goThrough = goThrough->next iar start_car_count il folosesc pentru a trece prin lista pentru a face suma greutatii vagoanelor din secventa.
Cand am format o secventa de cars_no vagoane, se compara cu heavy ce reprezinta ultima valoare gasita si salvez in start_car instant dupa ce atribui o valoare noua lui heavy.

order_train - am folosit ca algoritm de sortare bubble sort avand ca avantaj manipularea a 2 noduri vecine. Nu este cel mai eficient algoritm deoarece numarul de comparatii
este destul de mare. First este double pointer pentru a nu pierde legatura dintre noduri in urma interschimbarii (pointer la pointerul train_cars) .

fix_overload_train - caut trenul in gara care este supraincarcat, caut vagonul ce trebuie sters si salvez in variabila saveIt vagonul (power - abs trebuie sa fie >= 0 si
trebuie sa fie < decat optim adica power - abs gasit mai devreme . Se face o stergere simpla a vagonului prin eliberarea de memorie si facand legatura intre vecinii lui saveIt.



