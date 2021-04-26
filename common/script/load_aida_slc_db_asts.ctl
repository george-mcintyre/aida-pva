load data
infile '/mccdev/nfs_slcdb/aida_slc_db_asts.dat'
insert
into table aida_slc_db_asts
(
    instance  position(1:18) char,
    attribute position(20:23) char 
)
