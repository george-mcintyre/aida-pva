define inst = &1;
define attr = &2;

delete from aida_directory where name_id in (select id from aida_names where (instance = '&inst' and attribute = '&attr'));

delete from aida_names where (instance = '&inst' and attribute = '&attr');
