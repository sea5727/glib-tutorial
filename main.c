#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gprintf.h>


typedef struct tagInfo {
    char szID[12];
    int nValue1;
    int nValue2;
} StructInfo;

void iterator(gpointer key, gpointer value, gpointer user_data){
    StructInfo * pInfo = (StructInfo *) value;
     printf(user_data, *(gint*)key, pInfo->szID, pInfo->nValue1, pInfo->nValue2);
}


// g_hash_table_foreach함수에서 호출하는 또 하나의 함수
// hash table의 모든 값들을 파일에 저장하는 기능 
void iterator_filesave(gpointer key, gpointer value, gpointer user_data) 
{
    FILE * fp = (FILE *)user_data;
    StructInfo * pTemp = (StructInfo * )value;
    fprintf(fp, "%s %d %d\n",pTemp->szID, pTemp->nValue1, pTemp->nValue2);
}



// g_hash_table_new_full에 넣어 놓으면
// hash table의 값이 제거될 때 호출됨.
// key_destroy_func
void free_key(gpointer data)
{
    printf("free_key\n");
    g_free(data);
}


// g_hash_table_new_full에 넣어 놓으면
// hash table의 값이 제거될 때 호출됨.
// value_destroy_func
void free_data(gpointer data)
{
    printf("free_data\n");
    free(data);
}


// void hash_test()
// {
//     StructInfo * pInfo, *pTemp;
//     int num = 0;
//     gpointer gp;
//     FILE * fp;

//     // Hash table에서 값이 제거될 때 free_gdata(key 제거), free_data(value 제거)함수가 호출됨.
//     GHashTable* hash = g_hash_table_new_full(g_int_hash, g_int_equal, free_gdata, free_data);
//  while (1)
//     {
//         pInfo = malloc(sizeof(StructInfo));
//         //printf("struct : %p\n", pInfo);

//         // 콘솔에서 값 입력
//         scanf("%s %d %d", pInfo->szID, &(pInfo->nValue1), &(pInfo->nValue2));
//         if (pInfo->szID[0] == 'Q') // Q를 입력받으면 종료
//         {
//             free(pInfo);
//             break;
//         }
//         gint * k_num = g_new(gint, 1);
//         //printf("gint : %p\n", k_num);

//          // 편의상 키값은 ID의 번호로 설정하여 테스트 (8자리 중 맨 마지막 번호 사용 -> ID---001)
//         *k_num = pInfo->szID[7] - '0';

//         // 키값이 존재하는 지 확인하여 
//         gp = g_hash_table_lookup(hash, k_num);
//         if (gp == NULL)  // 존재하지 않으면 그냥 삽입
//         {
//             g_hash_table_insert(hash, k_num, pInfo);
//         }
//         else  // 존재한다면 원래 있던 value에 새로운 value를 더해서, 원래 있던 테이블의 값을 교체함. 
//         {
//             pTemp = (StructInfo *)gp;
//             pInfo->nValue1 += pTemp->nValue1;
//             pInfo->nValue2 += pTemp->nValue2;

//             // 교체 시 원래 있던 데이터가 자동으로 삭제되고, 새로운 데이터가 삽입되는 것으로 보임. 
//             // g_hash_table_new_full함수에 넣어 놓은 free함수들이 호출됨. 
//             g_hash_table_replace(hash, k_num, pInfo);
//         }
//     }

//     // hash table에 들어있는 값들 모두 출력
//     g_hash_table_foreach(hash, (GHFunc)iterator, "KEY: %d, Info : %s %d %d\n");

//     // hash table에 들어있는 값들을 모두 텍스트 파일로 출력
//     fp = fopen("output.txt", "wt");
//     g_hash_table_foreach(hash, (GHFunc)iterator_filesave, fp);
//     fclose(fp);

//     // hash talbe 제거 
//     g_hash_table_destroy(hash);
// }

void
add_test(){
    gint newkey = 100;
    GHashTable* hash = g_hash_table_new_full(g_int_hash, g_int_equal, free_key, free_data);
    gboolean ret = g_hash_table_add(hash, &newkey);
    g_print("ret:%d\n", ret);

    gint searchkey = 100;
    gpointer gp = g_hash_table_lookup(hash, &searchkey);
    if(gp == NULL){
        g_print("NULL\n");
    } else {
        g_print("NOT NULL\n");
    }
}
int 
main(int argc, char* argv) {
    add_test();
    return 0;
    GHashTable* hash = g_hash_table_new_full(g_int_hash, g_int_equal, free_key, free_data);
    StructInfo * pInfo;
    gint key = 10;
    gpointer gp = g_hash_table_lookup(hash, &key);
    
    guint size = g_hash_table_size(hash);
    g_print("size:%d\n", size);

    if(gp == NULL){
        pInfo = malloc(sizeof(StructInfo));
        snprintf(pInfo->szID, sizeof(pInfo->szID), "%s", "Student001");
        pInfo->nValue1 = 1;
        pInfo->nValue2 = 2;
        g_hash_table_insert(hash, &key, pInfo);

    } 
    size = g_hash_table_size(hash);
    g_print("size:%d\n", size);
    gp = g_hash_table_lookup(hash, &key);

    pInfo = (StructInfo *)gp;
    g_print("Id:%s, Value1:%d, Value:%d\n", 
        pInfo->szID, 
        pInfo->nValue1, 
        pInfo->nValue2);
    
    size = g_hash_table_size(hash);
    g_print("size:%d\n", size);
    
    
    gint newkey = 100;
    gboolean already = g_hash_table_contains(hash, &newkey);
    g_print("already :%d\n", already );
    if(!already) {
        gboolean ret = g_hash_table_add(hash, &newkey);
        g_print("ret:%d\n", ret);
    }

    gboolean ret = g_hash_table_add(hash, &newkey);
    g_print("ret:%d\n", ret);

    already = g_hash_table_contains(hash, &newkey);
    g_print("already :%d\n", already );
    if(!already) {
        gboolean ret = g_hash_table_add(hash, &newkey);
        g_print("ret:%d\n", ret);
    }

    return 0;
}
