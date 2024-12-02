extern void call_svc(void); // per chiamata nel template.s

int main(void) {
    call_svc(); // Richiama SVC per codificare il messaggio
    while (1);
}
