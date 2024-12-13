# Prova do módulo 4

## Objetivo

&nbsp;&nbsp;&nbsp;&nbsp;Criar um semáforo que possua modo diurno e noturno e corresponda aos comandos de um botão.

## Implementação
&nbsp;&nbsp;&nbsp;&nbsp;Primeiramente, todo o código foi padronizado para o português e as entradas e saídas foram declaradas corretamente na inicialização , conforme pode ser visto no arquivo [](). Além disso, a montagem do hardware foi corrigida.

&nbsp;&nbsp;&nbsp;&nbsp;Quando estiver escuro (segundo a leitura analógica do sensor LDR), o protótipo ativa o modo noturno e pisca o led amarelo a cada segundo.

<img src="/modo_noturno.png" width="100%">

&nbsp;&nbsp;&nbsp;&nbsp;Quando estiver claro (segundo a leitura analógica do sensor LDR), o protótipo ativa o modo convencional e alterna entre verde (3 segundos), amarelo (2 segundos) e vermelho (5 segundos):

<img src="/modo_diurno.png" width="100%">

&nbsp;&nbsp;&nbsp;&nbsp;Quando estiver claro (segundo a leitura analógica do sensor LDR) e o semáforo estiver no estado fechado (somente led vermelho aceso) e o botão for pressionado, o semáforo abre 1 segundo após o pressionamento do botão.

<img src="/botao.png" width="100%">

&nbsp;&nbsp;&nbsp;&nbsp;O protótipo suporta debounce na leitura do botão:

```cpp
    int leituraBotao = digitalRead(pino_botao);

    if (leituraBotao != ultimo_estado_botao) {
        ultimo_tempo_debounce = millis();
    }

    if ((millis() - ultimo_tempo_debounce) > 200) {
        if (leituraBotao == LOW && estado_botao == HIGH) {
            delay(1000);
            tempoFechado = tempoQueFechou + 6000;
        }
        estado_botao = leituraBotao;
    }
  }
```
