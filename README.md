# Caça ao Tesouro — CI1058 Redes 1 (2025/1)

**Universidade Federal do Paraná**  
**Curso: Bacharelado em Ciência da Computação**  
**Disciplina: CI1058 - Redes 1**  
**Professores:** Luiz Albini e Eduardo Todt  
**Trabalho T1 — Versão 20250424**

## 📜 Descrição

Este projeto consiste na implementação de um jogo de **caça ao tesouro** usando um modelo **cliente-servidor** com **comunicação em nível 2 (camada de enlace)**, utilizando **raw sockets**.

- O cliente e o servidor **devem ser executados em máquinas distintas**, conectadas diretamente por **cabo de rede**.
- O **servidor** controla o mapa e os tesouros, exibindo:
  - Tesouros sorteados.
  - Log de movimentos do jogador.
  - Posição atual do jogador.
- O **cliente** exibe um **grid 8x8**, onde o jogador movimenta seu agente para encontrar tesouros.

## 🎯 Objetivos Técnicos

- Comunicação entre cliente e servidor usando **protocolo próprio** baseado no Kermit.
- Transferência de arquivos (imagens, vídeos ou textos) como tesouros.
- Exibição dos tesouros no cliente conforme o tipo de arquivo.
- Marcação no mapa das posições visitadas e onde tesouros foram encontrados.
- Implementação com controle de fluxo **stop-and-wait** (janela deslizante opcional para pontos extras).

## 🗺️ Regras do Jogo

- O mapa é um **grid 8x8**, com o canto inferior esquerdo em (0,0).
- O servidor sorteia 8 posições com tesouros ao iniciar.
- Tesouros são arquivos nomeados de `1.xxx` até `8.xxx` (em `objetos/`), onde `xxx` pode ser `.txt`, `.jpg`, ou `.mp4`.
- Ao alcançar uma posição com tesouro, o servidor envia o arquivo ao cliente, que deve exibir seu conteúdo adequadamente.
- Movimentos inválidos (fora do grid) devem ser ignorados (ou opcionalmente sinalizados).

## 📡 Protocolo de Comunicação

### Estrutura do Frame

| Campo             | Tamanho  | Descrição                                  |
|------------------|----------|---------------------------------------------|
| Marcador início  | 8 bits   | Valor fixo: `01111110`                      |
| Tamanho          | 7 bits   | Tamanho do campo de dados (0 a 127 bytes)   |
| Sequência        | 5 bits   | Número de sequência                         |
| Tipo             | 4 bits   | Tipo da mensagem                           |
| Checksum         | 8 bits   | Sobre tamanho, sequência, tipo e dados      |
| Dados            | 0–127 B  | Dados da mensagem                           |

### Tipos de Mensagem

| Código | Tipo                     |
|--------|--------------------------|
| 0      | ACK                      |
| 1      | NACK                     |
| 2      | OK + ACK                 |
| 3      | Livre                    |
| 4      | Tamanho                  |
| 5      | Dados                    |
| 6      | Texto + ACK + Nome       |
| 7      | Vídeo + ACK + Nome       |
| 8      | Imagem + ACK + Nome      |
| 9      | Fim de Arquivo           |
| 10     | Mover Direita            |
| 11     | Mover Cima               |
| 12     | Mover Baixo              |
| 13     | Mover Esquerda           |
| 14     | Livre                    |
| 15     | Erro                     |

### Códigos de Erro

| Código | Descrição                      |
|--------|-------------------------------|
| 0      | Sem permissão de acesso        |
| 1      | Espaço insuficiente            |

## ⚙️ Considerações Técnicas

- Todos os campos devem usar tipos **unsigned** (`uchar` recomendado).
- **Não usar float** nos campos do protocolo.
- Para controle de fluxo, implemente **stop-and-wait**. Implementações com **go-back-N com janela 3** recebem pontos extras.
- É necessário tratar **timeouts** e **erros de comunicação**.
- Cálculo de espaço em disco deve ser feito com `statvfs`.
- Para checar arquivos e seus tamanhos, use `stat`.

## 🧪 Requisitos de Entrega

- Implementação em **C, C++ ou Go**.
- Código deve seguir o protocolo definido.
- **Relatório no formato de artigo SBC** deve ser entregue impresso no dia da apresentação.
- **Submissão no UFPR Virtual**.
- A apresentação será **em laboratório do DINF**, todos do grupo devem estar presentes e dominar o conteúdo.


