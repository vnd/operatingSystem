void dummy_test_entrypoint( void ) {


}

void main( void ) {
	char* video_memory = ( char* ) 0xb8000;
	*video_memory = 'X';
}