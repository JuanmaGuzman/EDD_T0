#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Candidate {
    int id;
    int votes;
    int status;  // 0 = eliminated; 1 = in_carrer
    int winner;  // 0 = no; 1 = yes.
};

struct Vote {
    int voterID;
    int candidateID;
};

/* Retorna true si ambos strings son iguales */
bool string_equals(char *string1, char *string2) {
  return !strcmp(string1, string2);
}

/* Revisa que los parametros del programa sean válidos */
bool check_arguments(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    printf("\tOUTPUT es la ruta del archivo de output\n");
    return false;
  }

  return true;
}

int compare_candidates(const void* p1, const void* p2) {
      const struct Candidate* c1 = *(const struct Candidate**) p1;
      const struct Candidate* c2 = *(const struct Candidate**) p2;
      if (c1->votes != c2->votes) {
          return c2->votes - c1->votes;
      } else {
          return c1->id - c2->id;
      }
  }

int main(int argc, char *argv[]) {
  /* Si los parámetros del programa son inválidos */
  if (!check_arguments(argc, argv)) {
    /* Salimos del programa indicando que no terminó correctamente */
    return 1;
  }

  FILE *input_file, *output_file;

  input_file = fopen(argv[1], "r");
  if (input_file == NULL) {
    printf("Could not open file %s\n", argv[1]);
    return 1;
  }

  output_file = fopen(argv[2], "w");

  /* Leemos la cantidad de candidatos y de eventos */
  int N_CANDIDATES;
  fscanf(input_file, "%d", &N_CANDIDATES);

  int N_EVENTS;
  fscanf(input_file, "%d", &N_EVENTS);

  int N_VOTERS = 0;

  /* Creamos las estructuras */
  struct Candidate *candidates = malloc(N_CANDIDATES * sizeof(struct Candidate));
  struct Vote *voters = NULL;  
  if (candidates == NULL) {
    printf("Error: no se pudo asignar memoria\n");
    return 1;
  }

  // Initialize candidates
  for (int i = 0; i < N_CANDIDATES; i++) {
      candidates[i].id = i;
      candidates[i].votes = 0;
      candidates[i].status = 1;
      candidates[i].winner = 0;
  }

  /* String para guardar la instrucción actual*/
  char command[32];

  for (int event = 0; event < N_EVENTS; event++) {
    fscanf(input_file, "%s", command);

    if (string_equals(command, "REGISTRAR-VOTO")) {
      int voterID, candidateID;
      fscanf(input_file, "%d %d", &voterID, &candidateID);
      voters = realloc(voters, (N_VOTERS + 1) * sizeof(struct Vote));
      voters[N_VOTERS].voterID = voterID;
      voters[N_VOTERS].candidateID = candidateID;
      N_VOTERS ++;
      candidates[candidateID].votes++;
      fprintf(output_file, "VOTO REGISTRADO %d\n", voterID);
    }
    else if (string_equals(command, "CONTEO-PARCIAL")) {
      fprintf(output_file, "CONTEO-PARCIAL\n");
      int total_votes = 0;
      for (int j = 0; j < N_CANDIDATES; j++) {
        fprintf(output_file, "\tCANDIDATE %d\n", candidates[j].id);
        total_votes += candidates[j].votes;
        int aux = 0;
        for (int k = 0; k < N_VOTERS; k++) {
          if (voters[k].candidateID == candidates[j].id) {
            fprintf(output_file, "\t\tVOTE %d\n", voters[k].voterID);
            aux += 1;
          }
        } if (aux == 0) {
            fprintf(output_file, "\t\tNO HAY VOTOS REGISTRADOS\n");
        }
      }
      fprintf(output_file, "TOTAL PARCIAL DE VOTOS: %d\n", total_votes);
    }
    else if (string_equals(command, "CONTEO-TOTAL")) {
      fprintf(output_file, "CONTEO-TOTAL\n");
      int total_votes = 0;
      for (int j = 0; j < N_CANDIDATES; j++) {
        fprintf(output_file, "\tCANDIDATO %d: %d\n", candidates[j].id, candidates[j].votes);
        total_votes += candidates[j].votes;
      }
      fprintf(output_file, "TOTAL VOTOS: %d\n", total_votes);
    } 
    else if (string_equals(command, "CONTEO-RANGO")) {
      int min_votes, max_votes;
      fscanf(input_file, "%d %d", &min_votes, &max_votes);
      /* [Por implementar] */
      fprintf(output_file, "CONTEO-RANGO %d %d\n", min_votes, max_votes);
      int total_votes_range = 0;
      for (int j = 0; j < N_CANDIDATES; j++) {
        if (candidates[j].votes >= min_votes && candidates[j].votes <= max_votes) {
          fprintf(output_file, "CANDIDATO %d: %d\n", candidates[j].id, candidates[j].votes);
          total_votes_range += candidates[j].votes;
        }
      }
      fprintf(output_file, "TOTAL DE VOTOS RANGO: %d\n", total_votes_range);
    }
    else if (string_equals(command, "ORDENAR-CANDIDATOS")) {
      int total_votes = 0;
      struct Candidate* candidate_ptrs[N_CANDIDATES];
      for (int i = 0; i < N_CANDIDATES; i++) {
          candidate_ptrs[i] = &candidates[i];
      }
      qsort(candidate_ptrs, N_CANDIDATES, sizeof(struct Candidate*), compare_candidates);
      fprintf(output_file, "CANDIDATOS-ORDENADOS\n");
      for (int i = 0; i < N_CANDIDATES; i++) {
          fprintf(output_file, "\tCANDIDATO %d: %d\n", candidate_ptrs[i]->id, candidate_ptrs[i]->votes);
          total_votes += candidate_ptrs[i]->votes;
      }
      fprintf(output_file, "TOTAL DE VOTOS: %d\n", total_votes);
  } 
    else if (string_equals(command, "ANULAR-VOTO")) {
      int voterID, candidateID;
      fscanf(input_file, "%d %d", &voterID, &candidateID);
      if (N_VOTERS == 0) {
        fprintf(output_file, "NO SE ENCONTRO UN VOTO VALIDO CON ID %d\n", voterID);
      }
      for (int i = 0; i < N_VOTERS; i++) {
        if (voters[i].voterID == voterID) {
          if (voters[i].candidateID == candidateID) {
            for (int j = 0; j < N_CANDIDATES; j++) {
              if (candidates[j].id == candidateID) {
                candidates[j].votes -= 1;
                fprintf(output_file, "VOTO ELIMINADO CORRECTAMENTE\n");
                for (int k = i; k < N_VOTERS - 1; k++) {
                  voters[k] = voters[k + 1];
                }
                N_VOTERS--;
                voters = realloc(voters, N_VOTERS * sizeof(struct Vote));
              } 
            }
          } else if (voters[i].voterID == voterID && voters[i].candidateID != candidateID) {
              fprintf(output_file, "NO SE ENCONTRO UN VOTO VALIDO CON ID %d\n", voters[i].voterID);
              break;
          }
        }
      }
    }
    else if (string_equals(command, "ELIMINAR-CANDIDATO")) {
      // Encontrar el candidato con menos votos y menor ID en caso de empate y que no esté eliminado y que no haya ganado.
      int eliminated_candidate_votes = N_VOTERS + 1;
      int eliminated_candidate_id = N_CANDIDATES + 1;

      for (int i = 0; i < N_CANDIDATES; i++) {
        if (candidates[i].votes < eliminated_candidate_votes) {
          if (candidates[i].status == 1) {
            // Si el candidato sigue en carrera puedo eliminarlo. 
            if (candidates[i].winner == 0) {
              // Si el candidato no ha ganado, puedo eliminarlo.
              eliminated_candidate_votes = candidates[i].votes;
              eliminated_candidate_id = candidates[i].id;
            }
          }
        }
      }

      struct Candidate* l1 = (struct Candidate*) malloc(N_CANDIDATES * sizeof(struct Candidate));
      struct Candidate* l2 = (struct Candidate*) malloc(N_CANDIDATES * sizeof(struct Candidate));

      // Construir L1.
      int j = 0;
      for (int i = 0; i < N_CANDIDATES; i++) {
          if (candidates[i].id >= eliminated_candidate_id) {
              l1[j] = candidates[i];
              j += 1;
          }
      }

      for (int i = 0; i < N_CANDIDATES; i++) {
          if (candidates[i].id < eliminated_candidate_id) {
              l1[j] = candidates[i];
              j += 1;
          }
      }

      // Encontrar el más cercano a L1.
      int l1_proxy = -1;
      int d1 = 0;
      for (int i = 0; i < N_CANDIDATES; i++) {
          if (l1[i].id != eliminated_candidate_id) {
              d1 += 1;
              if (l1[i].status == 1) {
                  if (l1[i].winner == 0) {
                      l1_proxy = l1[i].id;
                      break;
                  }
              }
          }
      }

      // Construir L2.
      j = 0;
      for (int i = 0; i < N_CANDIDATES; i++) {
          if (candidates[i].id > eliminated_candidate_id) {
              l2[j] = candidates[i];
              j += 1;
          }
      }

      for (int i = 0; i < N_CANDIDATES; i++) {
          if (candidates[i].id <= eliminated_candidate_id) {
              l2[j] = candidates[i];
              j += 1;
          }
      }

      // Encontrar el más cercano a L2.
      int l2_proxy = -1;
      int d2 = N_CANDIDATES;
      int i = 0;
      for (i; i < N_CANDIDATES; i++) {
          if (l2[i].id != eliminated_candidate_id) {
              d2 -= 1;
              if (l2[i].status == 1) {
                  if (l2[i].winner == 0) {
                      l2_proxy = l2[i].id;
                  }
              }
          }
      }

      int closest_id = -1;
      if (d1 == d2) {
          if (l1_proxy < l2_proxy) {
              closest_id = l1_proxy;
          } else {
              closest_id = l2_proxy;
          }
      } else if (d1 < d2) {
          closest_id = l1_proxy;
      } else if (d2 < d1) {
          closest_id = l2_proxy;
      }

      // Free the memory allocated for l1 and l2
      free(l1);
      free(l2);


      //Traspasamos los votos
      char message[100]; 
      for (int i = 0; i < N_CANDIDATES; i++) {
        if (candidates[i].id == closest_id) {
          candidates[i].votes += eliminated_candidate_votes;
          sprintf(message, "CANDIDATO %d HA RECIBIDO %d VOTOS", candidates[i].id, eliminated_candidate_votes);
        }
      }

      struct Vote* new_voters = (struct Vote*) malloc(N_VOTERS * sizeof(struct Vote));
      j = 0;
      for (int i = 0; i < N_VOTERS; i++) {
          if (voters[i].candidateID != eliminated_candidate_id) {
              new_voters[j] = voters[i];
              j++;
          }
      }
      for (int i = 0; i < N_VOTERS; i++) {
          if (voters[i].candidateID == eliminated_candidate_id) {
              voters[i].candidateID = closest_id;
              new_voters[j] = voters[i];
              j++;
          }
      }

      // Update original voters array with new order of votes
      memcpy(voters, new_voters, N_VOTERS * sizeof(struct Vote));
      free(new_voters);

      // Eliminamos candidato perdedor y dejamos con 0 votos. 
      for (int i = 0; i < N_CANDIDATES; i++) {
        if (candidates[i].id == eliminated_candidate_id) {
          candidates[i].votes = 0;
          candidates[i].status = 0;
          fprintf(output_file, "CANDIDATO %d HA SIDO ELIMINADO\n", candidates[i].id);
          fprintf(output_file, "%s\n", message);
        }
      }
    }
    else if (string_equals(command, "TRASPASAR-EXCESO-VOTOS")) {
      int candidateID, M;
      int votes_to_transfer = 0;
      fscanf(input_file, "%d %d", &candidateID, &M);
      //printf("Transfiere: %d\n", candidateID);

      for (int i = 0; i < N_CANDIDATES; i++) {
        if (candidates[i].id == candidateID) {
          // Marcamos que ganó, transferirá votos.
          candidates[i].winner = 1;
          if (candidates[i].votes > M) {
            votes_to_transfer = candidates[i].votes - M;
            int counter = 0;
            for (int j = 0; j < N_VOTERS; j++) {
              if (voters[j].candidateID == candidateID) {
                counter += 1;
                if (counter > M) {
                  // Marcamos a los votantes que les cambiaremos sus votos. 
                  voters[j].candidateID = -1;
                }
              }
            }
            candidates[i].votes -= votes_to_transfer;
          }
        }
      }

      struct Candidate* l1 = (struct Candidate*) malloc(N_CANDIDATES * sizeof(struct Candidate));
      struct Candidate* l2 = (struct Candidate*) malloc(N_CANDIDATES * sizeof(struct Candidate));

      // Construir L1.
      int j = 0;
      for (int i = 0; i < N_CANDIDATES; i++) {
          if (candidates[i].id >= candidateID) {
              l1[j] = candidates[i];
              j += 1;
          }
      }

      for (int i = 0; i < N_CANDIDATES; i++) {
          if (candidates[i].id < candidateID) {
              l1[j] = candidates[i];
              j += 1;
          }
      }

      // Encontrar el más cercano a L1.
      int l1_proxy = -1;
      int d1 = 0;
      for (int i = 0; i < N_CANDIDATES; i++) {
          if (l1[i].id != candidateID) {
              d1 += 1;
              if (l1[i].status == 1) {
                  if (l1[i].winner == 0) {
                      l1_proxy = l1[i].id;
                      break;
                  }
              }
          }
      }

      // Construir L2.
      j = 0;
      for (int i = 0; i < N_CANDIDATES; i++) {
          if (candidates[i].id > candidateID) {
              l2[j] = candidates[i];
              j += 1;
          }
      }

      for (int i = 0; i < N_CANDIDATES; i++) {
          if (candidates[i].id <= candidateID) {
              l2[j] = candidates[i];
              j += 1;
          }
      }

      // Encontrar el más cercano a L2.
      int l2_proxy = -1;
      int d2 = N_CANDIDATES;
      int i = 0;
      for (i; i < N_CANDIDATES; i++) {
          if (l2[i].id != candidateID) {
              d2 -= 1;
              if (l2[i].status == 1) {
                  if (l2[i].winner == 0) {
                      l2_proxy = l2[i].id;
                  }
              }
          }
      }

      int closest_id = -1;
      if (d1 == d2) {
          if (l1_proxy < l2_proxy) {
              closest_id = l1_proxy;
          } else {
              closest_id = l2_proxy;
          }
      } else if (d1 < d2) {
          closest_id = l1_proxy;
      } else if (d2 < d1) {
          closest_id = l2_proxy;
      }

      // Free the memory allocated for l1 and l2
      free(l1);
      free(l2);

      // Traspasar los votos según los candidatos marcados antes.
      struct Vote* new_voters = (struct Vote*) malloc(N_VOTERS * sizeof(struct Vote));
      j = 0;
      for (int i = 0; i < N_VOTERS; i++) {
          if (voters[i].candidateID != -1) {
              new_voters[j] = voters[i];
              j++;
          }
      }
      for (int i = 0; i < N_VOTERS; i++) {
          if (voters[i].candidateID == -1) {
              voters[i].candidateID = closest_id;
              new_voters[j] = voters[i];
              j++;
          }
      }

      // Update original voters array with new order of votes
      memcpy(voters, new_voters, N_VOTERS * sizeof(struct Vote));
      free(new_voters);

      // Actualizamos los votos de los votantes.
      for (int i = 0; i < N_VOTERS; i++) {
        if (voters[i].candidateID == -1) {
          voters[i].candidateID = closest_id;
        }
      }
      fprintf(output_file, "CANDIDATO %d HA SIDO ELEGIDO\n", candidateID);
      //Traspasamos los votos
      for (int i = 0; i < N_CANDIDATES; i++) {
        if (candidates[i].id == closest_id) {
          candidates[i].votes += votes_to_transfer;
        }
      }
      fprintf(output_file, "CANDIDATO %d HA RECIBIDO %d VOTOS\n", closest_id, votes_to_transfer);
    }
  }

  /*  [Por implementar] Liberamos nuestras estructuras */
  free(candidates);
  free(voters);

  fclose(input_file);
  fclose(output_file);

  return 0;
}
