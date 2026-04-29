# IAED - Invoicing System / Sistema de Faturação

[English](#english) | [Português](#português)

---

## English

### Project Overview
This project was developed as part of the **IAED (Introduction to Algorithms and Data Structures)** course at **Instituto Superior Técnico (IST)**. The objective is to implement a robust and high-performance invoicing management system capable of handling products, clients, and transactions with strict time and memory complexity requirements.

### Key Objectives
- Implement a command-line driven business logic for a retail environment.
- Manage dynamic data with a focus on memory safety and efficiency.
- Optimize search and insertion operations to handle large datasets.

### Optimization and Data Structures
A major focus of this implementation is the **hybrid data architecture**. To satisfy the performance requirements, the system combines:
- **AVL Trees**: Used for O(log N) efficient lookups of products and clients by their unique identifiers.
- **Doubly Linked Lists**: Used for O(1) chronological insertions and efficient bidirectional traversals of baskets and invoice histories.

This hybrid approach ensures that while searches remain logarithmic, maintaining the order of transactions and list iterations stays constant or linear with minimal overhead.

### Command Reference
The system processes commands from standard input:
- `p`: Add or update a product in the system.
- `l`: List products, supporting wildcard patterns (`*`) for descriptions.
- `a`: Add or remove products from the current temporary shopping basket.
- `r`: Display global system statistics and reports.
- `f`: Finalize the current basket, issuing an invoice and linking it to a client.
- `c`: List all invoices or filter them by a specific client.
- `d`: Delete a specific invoice or remove a quantity from a product's stock.
- `x`: Terminate the program and free all allocated resources.

---

## Português

### Resumo do Projeto
Este projeto foi desenvolvido no âmbito da Unidade Curricular de **IAED (Introdução aos Algoritmos e Estruturas de Dados)** no **Instituto Superior Técnico (IST)**. O objetivo é implementar um sistema de gestão de faturação robusto e de alta performance, capaz de gerir produtos, clientes e transações respeitando requisitos estritos de complexidade temporal e espacial.

### Objetivos Principais
- Implementar a lógica de negócio de um ambiente de retalho baseada em comandos.
- Gerir dados dinâmicos com foco na segurança de memória e eficiência.
- Otimizar operações de procura e inserção para lidar com grandes volumes de dados.

### Otimização e Estruturas de Dados
Um dos pilares deste desenvolvimento foi a utilização de uma **estrutura de dados híbrida**. Para cumprir os requisitos de performance, o sistema combina:
- **Árvores AVL**: Utilizadas para garantir procuras eficientes de O(log N) para produtos e clientes através dos seus identificadores únicos.
- **Listas Duplamente Ligadas**: Utilizadas para inserções cronológicas em O(1) e iterações bidirecionais eficientes em cestas de compras e históricos de faturas.

Esta abordagem híbrida permite que, enquanto as procuras permanecem logarítmicas, a manutenção da ordem das transações e as iterações de listas se mantenham constantes ou lineares com o mínimo de processamento adicional.

### Guia de Comandos
O sistema processa comandos a partir do input standard:
- `p`: Adiciona ou atualiza um produto no sistema.
- `l`: Lista produtos, suportando padrões de wildcard (`*`) nas descrições.
- `a`: Adiciona ou remove produtos da cesta de compras temporária atual.
- `r`: Exibe estatísticas globais e relatórios do sistema.
- `f`: Finaliza a cesta atual, emitindo uma fatura e associando-a a um cliente.
- `c`: Lista todas as faturas ou filtra-as por um cliente específico.
- `d`: Elimina uma fatura específica ou remove uma quantidade do stock de um produto.
- `x`: Termina o programa e liberta todos os recursos alocados.
