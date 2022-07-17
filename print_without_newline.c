int print_without_newline(char * string){
  int resp = 0;
  for (int i = 0; i < strlen(string); i++)
  {
    if(string[i] == '\n' ){
      printf("\\n");
    }else if(string[i] == '\r'){
      printf("\\r");
    }else{
      printf("%c",string[i]);
    }
  }
  return resp;
  
}


int has_newline(char * string){
  for (int i = 0; i < strlen(string); i++)
  {
    if(string[i] == '\n')return 1;
  }
  return 0;
}