# Strona przedmiotu Systemy Operacyjne

### Obejmuje przedmioty SOP1 / SOP2 / OPS1 / OPS2

Strona używa generatora Hugo (testowane wersje 0.80, ostatnio 0.117).

Żeby zbudować stronę do katalogu public/ wystarczy odpalić `hugo` bez argumentów. 

Z użyciem `hugo server` można uruchomić serwer dostarczający stronę.

`hugo deploy` na razie nie jest skonfigurowany. Docelowo pewnie będzie to jakiś `rsync` na serwer mini.

Do konwersji HTMLi wykopiowanych z drupala (kopiuję tylko ten fragment z pola edycji strony, a nie całość strony razem z dodatkami typu menu) na markdown'a używam `pandoc -f html -t gfm --wrap=none`. W przypadku tutoriali do lab nie konwertowałem, bo trzeba było zrobić milion ręcznych poprawek -- to skomplikowany kod. Inne rzeczy (np. wykłady) przechodzą raczej gładko. Trzeba tylko zaktualizować linki i pokopiować zasoby typu pdf ze slajdami.
