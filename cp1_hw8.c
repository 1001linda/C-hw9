#include<stdio.h>
#include<string.h>
#include<math.h>
enum state {normal = 0, poisonous = 1, sleep = 2, exhausted = 3, paralysis = 4, dead = 5};

typedef struct{
    char name[25];
    double atk;
    double hp;
    int spk;
    int a;
}Pekomon;

int checkAlive(Pekomon p){
    if(p.hp > 0){
        return 1; //alive
    }else{
        return 0;
    }
}

void attack(Pekomon *a, Pekomon *b){
    b->hp = b->hp - a->atk;
}

void halfAttack(Pekomon *a, Pekomon *b){
    double halvedAtk = ceil(a->atk/2);
//  printf("a's halved atk is %lf\n", halvedAtk);
    b->hp = b->hp - halvedAtk;
//  printf("b' hp after be halved attack by a: %lf\n", b->hp);
}

int main(){
    int n, k;
    scanf("%d %d", &n, &k);

    Pekomon alice[n];
    Pekomon bob[n];

    for(int i = 0; i < n; i++){
        char name[25];
        int spk, a;
        double hp, atk;
        scanf("%s %lf %lf %d %d", name, &atk, &hp, &spk, &a);
        strcpy(alice[i].name, name);
        alice[i].atk = atk;
        alice[i].hp = hp;
        alice[i].spk = spk;
        alice[i].a = a;
    }

    for(int i = 0; i < n; i++){
        char name[25];
        double atk, hp;
        int spk, a;
        scanf("%s %lf %lf %d %d", name, &atk, &hp, &spk, &a);
        strcpy(bob[i].name, name);
        bob[i].atk = atk;
        bob[i].hp = hp;
        bob[i].spk = spk;
        bob[i].a = a;
    }

    int a_idx = 0; //record alice's a_idxth pekomon
    int b_idx = 0;
    Pekomon *a = &alice[0]; //the pekomon alice currently use
    Pekomon *b = &bob[0];

    //enum state {normal = 0, poisonous = 1, sleep = 2, exhausted = 3, paralysis = 4, dead = 5};
    int a_spe = 0; //record alice's pekomon normal att, when a_spe = k, then trigger special att
    int b_spe = 0;
    int a_dur = 0; //record pekomon a's remain durataion of b's special att
    int b_dur = 0;

    enum state a_state = normal; //record alice's pekomon current state
    enum state b_state = normal;

    while(a_idx < n && b_idx < n){

        if(a_dur > 0){
            a_dur--;
            if(a_dur == 0){
                a_state = normal;
            }
        }
        if(b_dur > 0){
            b_dur--;
            if(b_dur == 0){
                b_state = normal;
            }
        }

        if(a_state == normal){
            attack(a, b);
            a_spe++;
        }else if(a_state == poisonous){
            a->hp = ceil(a->hp/2);
            attack(a, b);
            a_spe++;
        }else if(a_state == sleep){
            // do nothing
        }else if(a_state == exhausted){
            halfAttack(a, b);
            a_spe++;
        }else if(a_state == paralysis){
            a->hp = ceil(a->hp/2);
        }else if(a_state == dead){
            a_idx++;
            if(a_idx < n){
                a = &alice[a_idx];
                a_state = normal;
                a_spe = 0;
                a_dur = 0;
            }
        }


        if(a_spe == k){
            b_state = a->spk;
            if(b_state == dead){
                b_dur = 1;
            }else if(b_state == a->spk){
                b_dur += a->a; // if b is suffered from the same special effect
            }else{
                b_dur = a->a;
            }
            a_spe = 0; // restart a's number of attack to trigger the next time special attack
        }

        if(!checkAlive(*b)){
            b_state = dead;
        }


//              printf("a_state: %d, a_idx: %d, a_spe: %d, a_dur: %d, %s, hp: %lf, atk: %lf, spk: %d, a: %d\n", a_state, a_idx, a_spe, a_dur, a->name, a->hp, a->atk, a->spk, a->a);
//              printf("b_state: %d, b_idx: %d, b_spe: %d, b_dur: %d, %s, hp: %lf, atk: %lf, spk: %d, a: %d\n", b_state, b_idx, b_spe, b_dur, b->name, b->hp, b->atk, b->spk, b->a);
//              printf("\n");

        if(b_state == dead){
            b_idx++;
            if(b_idx < n){
                b = &bob[b_idx];
                b_state = normal;
                b_spe = 0;
                b_dur = 0;
            }
        }else if(b_state == normal){
            attack(b, a);
            b_spe++;
        }else if(b_state == poisonous){
            printf("b before poisonous hp: %lf\n", b->hp);
            b->hp = ceil(b->hp/2);
            printf("b after poisonous hp: %lf\n", b->hp);
            attack(b, a);
            b_spe++;
        }else if(b_state == sleep){
        // do nothing
        }else if(b_state == exhausted){
            halfAttack(b, a);
            b_spe++;
        }else if(b_state == paralysis){
            b->hp = ceil(b->hp/2);
        }

        if(b_spe == k){
            a_state = b->spk;
            if(a_state == dead){
                a_dur = 1;
            }else if(a_state == b->spk){
                a_dur += b->a; // if a is suffered from the same special effect
            }else{
                a_dur = b->a;
            }
                b_spe = 0; // restart b's number of attack to trigger the next time special attack
        }

        if(!checkAlive(*a)){
            a_state = dead;
        }

//              printf("a_state: %d, a_idx: %d, a_spe: %d, a_dur: %d, %s, hp: %lf, atk: %lf, spk: %d, a: %d\n", a_state, a_idx, a_spe, a_dur, a->name, a->hp, a->atk, a->spk, a->a);
//              printf("b_state: %d, b_idx: %d, b_spe: %d, b_dur: %d, %s, hp: %lf, atk: %lf, spk: %d, a: %d\n", b_state, b_idx, b_spe, b_dur, b->name, b->hp, b->atk, b->spk, b->a);
//              printf("\n");
    }

        //print winner
    if(a_idx > b_idx){
        // bob win
        // alice does not have any pekomon
        printf("Bob\n");
        //print the remain pekomon of bob
        for(int i = b_idx; i < n; i++){
            if(b_state == exhausted){
                printf("%s %d %d\n", bob[i].name, (int)ceil(bob[i].atk/2), (int)bob[i].hp);
            }else{
                printf("%s %d %d\n", bob[i].name, (int)bob[i].atk, (int)bob[i].hp);
            }
        }
    }else{
        // alice win
        // bob does not have any pekomon
        printf("Alice\n");
        //print the remain pekomon of alice
        for(int i = a_idx; i < n; i++){
            if(a_state == exhausted){
                printf("%s %d %d\n", alice[i].name, (int)ceil(alice[i].atk/2), (int)alice[i].hp);
            }else{
                printf("%s %d %d\n", alice[i].name, (int)alice[i].atk, (int)alice[i].hp);
            }
        }
    }
}