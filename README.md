# cg
Trabalho de CG

Victor Hugo Ota
Gustavo de Jesus Merli

1. Coleção de objetos de cena
	A coleção escolhida foi uma lista da biblioteca list com algumas alterações
	nas classes para que a adição e remoção fossem de tempo constante. A adição
	foi feita sempre no final da lista (complexidade constante independente do 
	tamanho da lista). A remoção foi implementada esperando um iterador como
	parâmetro (guardado na própria classe com nome myIterator), assim a remoção
	também teria complexidade constante independente do tamanho da lista.

2. Coleção de componentes
	A coleção de componentes foi implementada da mesma maneira da coleção de 
	objetos de cena com a diferença que a lista sempre teria como primeira 
	posição o componente transform.

3. keyInputEvent() reescrito
	A função keyInputEvent foi reescrita tirando o pause e o fechamento da
	janela (foram retirados os dois já que os atributos window e pause
	da classe GLWindow são privados) e adicionado a opção Delete com o
	comando Ctrl + Delete removendo o objeto de cena corrente.