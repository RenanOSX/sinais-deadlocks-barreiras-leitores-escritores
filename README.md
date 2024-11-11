# sinais-deadlocks-barreiras-leitores-escritores

# `leitor-escritor.c`

1. **Variáveis Globais**:
   - `shared_data`: dado compartilhado entre os threads.
   - `reader_count`: contador de leitores ativos.
   - `lock` e `write_lock`: mutexes para sincronizar o acesso aos dados e evitar conflitos.

2. **Função `reader` (Leitor)**:
   - Cada leitor incrementa `reader_count` com `lock`.
   - O primeiro leitor bloqueia `write_lock`, impedindo escritores de acessar o dado enquanto leitores estiverem lendo.
   - Ao finalizar, o leitor decrementa `reader_count`. Se for o último, libera `write_lock`.

3. **Função `writer` (Escritor)**:
   - O escritor adquire `write_lock` antes de alterar `shared_data` e o libera após a modificação, garantindo acesso exclusivo ao dado.

4. **Execução no `main`**:
   - Inicializa os mutexes.
   - Cria cinco threads leitores e dois escritores, executando-os simultaneamente.
   - Espera todos terminarem e, por fim, destrói os mutexes.

Essa estrutura permite leituras simultâneas e exclusividade para escrita, evitando conflitos e mantendo a integridade de `shared_data`.

# `sinais.c`

1. **Variáveis Globais**:
   - `ready`: indica se o sinal está pronto para ser recebido.
   - `lock`: mutex que controla o acesso à variável `ready`.
   - `cond`: condição usada para notificar o thread que está esperando.

2. **Função `waiter` (Esperador)**:
   - Esse thread trava `lock` e verifica `ready`.
   - Caso `ready` seja 0, ele entra em espera usando `pthread_cond_wait`, liberando `lock` e pausando até que `cond` seja sinalizado.
   - Quando recebe o sinal, ele prossegue e libera `lock`.

3. **Função `sender` (Enviador)**:
   - Esse thread trava `lock`, define `ready` como 1 e sinaliza a condição `cond`.
   - Isso acorda o thread em espera (`waiter`), que pode então continuar sua execução.
   - Após sinalizar, ele libera `lock`.

4. **Execução no `main`**:
   - Inicializa o mutex e a condição.
   - Cria o thread `waiter` (esperador) e o thread `sender` (enviador).
   - Aguarda ambos terminarem com `pthread_join`.
   - Por fim, destrói o mutex e a condição.

Esse código mostra a sincronização entre threads usando uma condição: o `waiter` espera um sinal para continuar, que é enviado pelo `sender` ao definir `ready = 1`.

# `deadlock.c`

1. **Mutexes**:
   - `lock1` e `lock2` são dois mutexes que representam recursos que os threads precisam adquirir em uma ordem específica.

2. **Função `thread1`**:
   - `thread1` adquire `lock1` e imprime uma mensagem.
   - Após um `sleep`, ele tenta adquirir `lock2`.
   - Quando `thread1` tem ambos os mutexes, ele os libera na ordem inversa.

3. **Função `thread2`**:
   - `thread2` adquire `lock2` primeiro e imprime uma mensagem.
   - Após um `sleep`, ele tenta adquirir `lock1`.
   - Assim como `thread1`, ele libera os mutexes na ordem inversa quando ambos estão adquiridos.

4. **Execução no `main`**:
   - Inicializa os mutexes e cria os dois threads (`t1` e `t2`).
   - Aguarda que ambos terminem com `pthread_join`.
   - Finalmente, destrói os mutexes.

### Explicando o Deadlock no código
O deadlock ocorre porque `thread1` e `thread2` esperam indefinidamente um pelo outro: `thread1` segura `lock1` enquanto espera `lock2`, e `thread2` segura `lock2` enquanto espera `lock1`. Isso simula uma situação de impasse, onde nenhum thread pode avançar.

# `barreira.c`

1. **Barreira**:
   - A barreira (`pthread_barrier_t barrier`) é configurada para permitir a passagem quando todas as `NUM_THREADS` threads atingirem o ponto de espera.

2. **Função `thread_func`**:
   - Cada thread imprime uma mensagem ao chegar na barreira e, em seguida, chama `pthread_barrier_wait`, aguardando que todas as outras threads também alcancem esse ponto.
   - Após todas as threads chegarem à barreira, elas continuam a execução e imprimem uma mensagem informando que passaram pela barreira.

3. **Execução no `main`**:
   - Inicializa a barreira com o número total de threads (`NUM_THREADS`).
   - Cria as threads, passando um identificador único (`thread_ids[i]`) para cada uma.
   - Usa `pthread_join` para esperar a conclusão de todas as threads.
   - Destrói a barreira após o término das threads.

Esse código sincroniza os threads usando uma barreira, garantindo que todas cheguem a um ponto comum antes de prosseguir, o que é útil para coordenar tarefas em fases.
