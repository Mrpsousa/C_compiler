// p-code.cpp : cpp.sh/73ali
//
// C conventions
// 0 == FALSE
//
#include <stdio.h>
#include <string.h>

#define levmax 3
#define amax 2047
#define maxIntrucoes 1024
int cont = 0;

typedef enum fct {LIT, OPR, LOD, STO, CAL, INT, JMP, JPC} fct;

typedef struct tinstruction {
     fct    f;
     int    l; // l: 0..levmax; // Level buffer
     int    a; // a: 0..amax;   // Address buffer
}instruction;
char * instructionString[] = { "LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC", "WRT" };

instruction code[2048];

//procedure interpret;
// const stacksize = 500;

#define stacksize 5012

 // p, b, t: integer; {program-, base-, topstack-registers}
 int p, // program-register
     b, // base-register
     t; // topstack-register
 
instruction i;            //: instruction; {instruction register}
int         s[stacksize]; //: array [1..stacksize] of integer; {datastore}

int base(int l){ //l: integer)
 int b1; //

 b1 = b; // {find base l levels down}
 while (l > 0) {
    b1 = s[b1];
    l  = l - 1;
 }
 return b1;
}//end-int-base

// Determina se seu argumento � impar
int odd(int x){ return (  ((x%2)==1) ); }

void pcodevhw(){ // begin 
 printf("\n start pl/0");
 printf("\n t   b   p     f   l   a                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14");
 printf("\n === === ===   === === ===             === === === === === === === === === === === === === === ===");
 t = -1; // topstack-register
 b = 0; // base-register
 p = 0; // program-register
 
 // OBS: s[0] must not be used
 s[1] = 0; 
 s[2] = 0; 
 s[3] = 0;
 
 do {
  i = code[p]; 
  //printf("\n %3d %3d %3d - %3d %3d %3d", t, b, p, i.f, i.l, i.a);
  printf("\n %3d %3d %3d   ", t, b, p);
  p = p + 1;
 // switch case i.f-BEGIN
  switch  (i.f) { // with i do case f of
    case LIT : { t = t + 1; s[t] = i.a; printf("LIT %3d %3d", i.l, i.a); } break;
    case OPR :
               printf("OPR %3d %3d", i.l, i.a);
//             switch case i.a-BEGIN
               switch (i.a) { // case a of {operator}
                 case  0: { // return
                            t = b - 1; 
                            p = s[t + 3]; 
                            b = s[t + 2];
                          }
                          break;
         
                 case  1: { //
                            s[t] = -s[t];
                          }
                          break;
         
                 case  2: { //  ADD     : PUSH( POP()+POP() )
                            t    = t - 1; 
                            s[t] = s[t] + s[t + 1];
                          }
                          break;
         
                 case  3: { // SUBSTRACT: PUSH( POP()-POP() )
                            t    = t - 1; 
                            s[t] = s[t] - s[t + 1];
                          }
                          break;
         
                 case  4: { // MULTIPLY: PUSH( POP()*POP() )
                            t    = t - 1; 
                            s[t] = s[t] * s[t + 1];
                          }
                          break;
         
                 case  5: { // DIVIDE  : PUSH( POP()/POP() )
                            t    = t - 1; 
                            s[t] = s[t] / s[t + 1];
                          }
                          break;
         
                 case  6: { // 
                            s[t] = odd(s[t]); //s[t] = ord(odd(s[t]));
                          }
                          break;
         
                 case  8: { // EQUAL
                            t    = t - 1; 
                            s[t] = (s[t] == s[t + 1]);
                          }
                          break;
         
                 case  9: { // NOT.EQUAL
                            t    = t - 1; 
                            s[t] = (s[t] != s[t + 1]);
                          }
                          break;
         
                 case 10: { // LESS THAN
                            t    = t - 1; 
                            s[t] = (s[t] < s[t + 1]);
                          }
                          break;
         
                 case 11: { // GREATER EQUAL THAN
                            t    = t - 1; 
                            s[t] = (s[t] >= s[t + 1]);
                          }
                          break;
         
                 case 12: { // GREATER THAN
                            t = t - 1; 
                            s[t] = (s[t] > s[t + 1]);
                          }
                          break;
         
                 case 13: { // LESS EQUAL THAN
                            t    = t - 1; 
                            s[t] = (s[t] <= s[t + 1]);
                          }
                          break;
               } // end;
//         switch case i.a-END
           break;

   case LOD : { // LOAD
                printf("LOD %3d %3d", i.l, i.a);
                t    = t + 1; 
                s[t] = s[base(i.l) + i.a];
              }
              break;

   case STO : { // STORE
                printf("STO %3d %3d", i.l, i.a);
                s[base(i.l)+i.a] = s[t]; 
                t            = t - 1;
              }
              break;

   case CAL :
              { // {generate new block mark}
                printf("CAL %3d %3d", i.l, i.a);
                s[t + 1] = base(i.l); 
                s[t + 2] = b; 
                s[t + 3] = p;
                b        = t + 1; 
                p        = i.a;
              }
              break;

   case INT : t = t + i.a;printf("INT %3d %3d", i.l, i.a); break;
   case JMP : p = i.a;    printf("JMP %3d %3d", i.l, i.a); break;
   case JPC : if (s[t] == 1) { p = i.a; }  t = t - 1; printf("JPC %3d %3d", i.l, i.a); break;
  } // end {with, case}
  // switch case i.f-END
  
  // print stack
  printf("      s[] : ");
  for (int h=0; h<=t; h++) { printf(" %3d", s[h]); }
 } while ( p != 0 );

 printf("\n === === ===   === === ===             === === === === === === === === === === === === === === ==="); 
 printf("\n t   b   p     f   l   a                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14");
 printf("\n end pl/0");
}//end-void-pcmachine() {interpret};

int getInstructionCode(char *c){
	int i;
	for (i = 0; i<9;i++){
		if(strcmp(c, instructionString[i]) == 0){
			return i;
		}
	}
	return -1;
}

char * getInstructionName(int inst){
	return instructionString[inst];
}

void setInstruction(fct func, int level, int arg){
	code[cont].f = func;
	code[cont].l = level;
	code[cont++].a = arg;
}

int main(){
	char a[4];    
    int b = 0, c = 0, totalInstrucoes = 0;
    int codeOperation;
    for(totalInstrucoes = 0; totalInstrucoes < maxIntrucoes; totalInstrucoes++){
	    scanf("\n%[^ ] %d %d",a,&b,&c); 
	    
        codeOperation = getInstructionCode(a);
        if(codeOperation != -1){
            instruction instrucao;
            instrucao.f = codeOperation;
            instrucao.l = b;
            instrucao.a = c;
            
            code[totalInstrucoes] = instrucao;
        }else{
            if(strcmp(a, "FIM") == 0){
                break;
            }
            printf("Instrucao desconhecida\n");
        }        
    }

	// // Aqui vc preenche as instrucoes no vetor code
  //   code[ 0].f = JMP; code[ 0].l = 0; code[ 0].a = 19;
  //   code[ 1].f = INT; code[ 1].l = 0; code[ 1].a = 5;
  //   code[ 2].f = LOD; code[ 2].l = 0; code[ 2].a = 3;
  //   code[ 3].f = LIT; code[ 3].l = 0; code[ 3].a = 1;
  //   code[ 4].f = OPR; code[ 4].l = 0; code[ 4].a = 8;
  //   code[ 5].f = JPC; code[ 5].l = 0; code[ 5].a = 16;
  //   code[ 6].f = LOD; code[ 6].l = 0; code[ 6].a = 3;
  //   code[ 7].f = LIT; code[ 7].l = 0; code[ 7].a = 1;
  //   code[ 8].f = OPR; code[ 8].l = 0; code[ 8].a = 3;
  //   code[ 9].f = STO; code[ 9].l = 0; code[ 9].a = 8;
  //   code[10].f = CAL; code[10].l = 0; code[10].a = 1;
  //   code[11].f = LOD; code[11].l = 0; code[11].a = 3;
  //   code[12].f = LOD; code[12].l = 0; code[12].a = 4;
  //   code[13].f = OPR; code[13].l = 0; code[13].a = 4;
  //   code[14].f = STO; code[14].l = 1; code[14].a = 4;
  //   code[15].f = OPR; code[15].l = 0; code[15].a = 0;
  //   code[16].f = LIT; code[16].l = 0; code[16].a = 1;
  //   code[17].f = STO; code[17].l = 1; code[17].a = 4;
  //   code[18].f = OPR; code[18].l = 0; code[18].a = 0;
  //   code[19].f = INT; code[19].l = 0; code[19].a = 5;
  //   code[20].f = LIT; code[20].l = 0; code[20].a = 2;
  //   code[21].f = STO; code[21].l = 0; code[21].a = 8;
  //   code[22].f = CAL; code[22].l = 0; code[22].a = 1;
  //   code[23].f = OPR; code[23].l = 0; code[23].a = 0;

	// Aqui vc chama a P-code machine para interpretar essas instrucoes
	pcodevhw();
	return 0;
}

/*
 start pl/0
 t   b   p     f   l   a                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14
 === === ===   === === ===             === === === === === === === === === === === === === === ===
  -1   0   0   JMP   0  19      s[] : 
  -1   0  19   INT   0   5      s[] :    0   0   0   0   0
   4   0  20   LIT   0   2      s[] :    0   0   0   0   0   2
   5   0  21   STO   0   8      s[] :    0   0   0   0   0
   4   0  22   CAL   0   1      s[] :    0   0   0   0   0
   4   5   1   INT   0   5      s[] :    0   0   0   0   0   0   0  23   2   0
   9   5   2   LOD   0   3      s[] :    0   0   0   0   0   0   0  23   2   0   2
  10   5   3   LIT   0   1      s[] :    0   0   0   0   0   0   0  23   2   0   2   1
  11   5   4   OPR   0   8      s[] :    0   0   0   0   0   0   0  23   2   0   0
  10   5   5   JPC   0  16      s[] :    0   0   0   0   0   0   0  23   2   0
   9   5   6   LOD   0   3      s[] :    0   0   0   0   0   0   0  23   2   0   2
  10   5   7   LIT   0   1      s[] :    0   0   0   0   0   0   0  23   2   0   2   1
  11   5   8   OPR   0   3      s[] :    0   0   0   0   0   0   0  23   2   0   1
  10   5   9   STO   0   8      s[] :    0   0   0   0   0   0   0  23   2   0
   9   5  10   CAL   0   1      s[] :    0   0   0   0   0   0   0  23   2   0
   9  10   1   INT   0   5      s[] :    0   0   0   0   0   0   0  23   2   0   5   5  11   1   0
  14  10   2   LOD   0   3      s[] :    0   0   0   0   0   0   0  23   2   0   5   5  11   1   0   1
  15  10   3   LIT   0   1      s[] :    0   0   0   0   0   0   0  23   2   0   5   5  11   1   0   1   1
  16  10   4   OPR   0   8      s[] :    0   0   0   0   0   0   0  23   2   0   5   5  11   1   0   1
  15  10   5   JPC   0  16      s[] :    0   0   0   0   0   0   0  23   2   0   5   5  11   1   0
  14  10  16   LIT   0   1      s[] :    0   0   0   0   0   0   0  23   2   0   5   5  11   1   0   1
  15  10  17   STO   1   4      s[] :    0   0   0   0   0   0   0  23   2   1   5   5  11   1   0
  14  10  18   OPR   0   0      s[] :    0   0   0   0   0   0   0  23   2   1
   9   5  11   LOD   0   3      s[] :    0   0   0   0   0   0   0  23   2   1   2
  10   5  12   LOD   0   4      s[] :    0   0   0   0   0   0   0  23   2   1   2   1
  11   5  13   OPR   0   4      s[] :    0   0   0   0   0   0   0  23   2   1   2
  10   5  14   STO   1   4      s[] :    0   0   0   0   2   0   0  23   2   1
   9   5  15   OPR   0   0      s[] :    0   0   0   0   2
   4   0  23   OPR   0   0      s[] : 
 === === ===   === === ===             === === === === === === === === === === === === === === ===
 t   b   p     f   l   a                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14
 end pl/0 
Exit code: 0 (normal program termination)
*/